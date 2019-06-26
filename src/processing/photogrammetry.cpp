#include <processing/photogrammetry.hpp>
#include <processing/epipolar.hpp>
#include <processing/image.hpp>
#include <spdlog/spdlog.h>
#include <Eigen/Eigen>
#include <opengl/mygl_glfw.hpp>
#include <future>
#include <spdlog/spdlog.h>

#ifdef __ANDROID__
#include <EGL/egl.h>
#include <spdlog/sinks/android_sink.h>
#endif

namespace mpp
{
    photogrammetry_processor::photogrammetry_processor()
    {
        _detection_settings.octaves = 4;
        _detection_settings.feature_scales = 3;
        _detection_settings.orientation_magnitude_threshold = 0.0002f;
        _detection_settings.orientation_slices = 18;

        _match_settings.relation_threshold = 0.8f;
        _match_settings.similarity_threshold = 0.83f;
        _match_settings.max_match_count = 50;
    }
    void photogrammetry_processor::clear()
    {
        _image_matches.clear();
        _images.clear();
    }
    void photogrammetry_processor::add_image(std::shared_ptr<image> img, float focal_length)
    {
        if (!_sift_cache)
            _sift_cache = sift::create_cache(_detection_settings.octaves, _detection_settings.feature_scales);

        const auto [insert_iter, did_emplace] = _images.emplace(std::move(img), image_info{});
        constexpr auto max_width = 400;
        auto& imgref = *insert_iter->first;
        const float aspect = float(imgref.dimensions().x) / imgref.dimensions().y;
        const auto w = max_width;
        const auto h = int(aspect * max_width);
        if (did_emplace)
        {
            insert_iter->second.feature_points = sift::detect_features(*_sift_cache, image(imgref).resize(w, h), _detection_settings, sift::dst_system::normalized_coordinates);
            insert_iter->second.camera_intrinsics = glm::mat3(1.f);
            insert_iter->second.camera_intrinsics[0][0] = focal_length;
            insert_iter->second.camera_intrinsics[1][1] = focal_length;
        }
    }

    void photogrammetry_processor::match_all()
    {
        for (auto& i : _images)
            _image_matches[i.first];

        std::for_each(_images.begin(), _images.end(), [&](const std::pair<std::shared_ptr<image>, image_info>& a) {
            std::for_each(std::next(_images.find(a.first)), _images.end(), [&](const std::pair<std::shared_ptr<image>, image_info>& b) {
                const auto matches = sift::match_features(a.second.feature_points, b.second.feature_points, _match_settings);
                spdlog::info("{} matches.", matches.size());
                if (matches.size() >= 8)
                {
                    auto& insert = _image_matches[a.first][b.first];
                    insert.match_points = sift::corresponding_points(matches);
                    insert.fundamental_matrix = ransac_fundamental(insert.match_points);
                }
                });
            });
    }
    std::optional<glm::mat3> photogrammetry_processor::fundamental_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b)
    {
        bool is_a = true;
        auto it = _image_matches.find(a);
        if (it == _image_matches.end())
        {
            it = _image_matches.find(b);
            is_a = false;
        }

        if (it == _image_matches.end())
            return std::nullopt;

        auto oit = it->second.find(is_a ? b : a);
        if (oit == it->second.end())
        {
            if (is_a)
            {
                it = _image_matches.find(b);
                if (it != _image_matches.end())
                {
                    oit = it->second.find(a);
                    if (oit != it->second.end())
                        return oit->second.fundamental_matrix;
                }
            }
            return std::nullopt;
        }
        return oit->second.fundamental_matrix;
    }
    std::optional<glm::mat3> photogrammetry_processor::essential_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b)
    {
        const auto fund = fundamental_matrix(a, b);
        if (fund)
        {
            const auto& k_a = _images[a].camera_intrinsics;
            const auto& k_b = _images[a].camera_intrinsics;
            return transpose(inverse(k_b)) * *fund * inverse(k_a);
        }
        return std::nullopt;
    }
    std::optional<glm::mat4> photogrammetry_processor::relative_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b)
    {
        const auto e = essential_matrix(a, b);
        if (!e) return std::nullopt;

        Eigen::Matrix3f e_mat = reinterpret_cast<const Eigen::Matrix<float, 3, 3, Eigen::ColMajor>&>(e);
        Eigen::JacobiSVD<Eigen::Matrix3f> svd(e_mat, Eigen::ComputeFullU | Eigen::ComputeFullV);

        Eigen::Matrix3f w;
        w << 0, -1, 0,
            1, 0, 0,
            0, 0, 1;
        Eigen::Matrix3f w_inv;
        w_inv = w.transpose();
        Eigen::Matrix3f z;
        z << 0, 1, 0,
            -1, 0, 0,
            0, 0, 0;

        const Eigen::Matrix<float, 3, 3, Eigen::ColMajor> r = svd.matrixU() * w_inv * svd.matrixV();
        const Eigen::Matrix3f tx = svd.matrixU() * z * svd.matrixU().transpose();

        glm::mat4 trafo(reinterpret_cast<const glm::mat3&>(r));
        trafo[3][0] = tx(2, 1);
        trafo[3][1] = tx(0, 2);
        trafo[3][2] = tx(1, 0);
        trafo[3][3] = 1.f;

        return trafo;
    }
    std::vector<photogrammetry_processor::transformed_image> photogrammetry_processor::build_flat_hierarchy()
    {
        std::vector<transformed_image> imgs;
        visit_match_tree([&](const std::shared_ptr<image>& img, glm::mat4 tf) {
            imgs.emplace_back(transformed_image{ img, tf });
            });
        return imgs;
    }
    void photogrammetry_processor_async::run()
    {
        std::promise<void> p;
        const auto fut = p.get_future();
        _worker = std::thread([this, prom = std::move(p)]() mutable {
            std::unique_lock<std::mutex> lock(_proc_mtx);

#ifdef __ANDROID__
            EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            int vmaj, vmin;
            eglInitialize(display, &vmaj, &vmin);
            // Step 3 - Make OpenGL ES the current API.
            eglBindAPI(EGL_OPENGL_ES_API);

            const EGLint attrib_list[] = {
                // this specifically requests an Open GL ES 2 renderer
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                // (ommiting other configs regarding the color channels etc...
                EGL_NONE
            };

            EGLConfig config;
            EGLint num_configs;
            eglChooseConfig(display, attrib_list, &config, 1, &num_configs);

            // ommiting other codes

            const EGLint context_attrib_list[] = {
                // request a context using Open GL ES 2.0
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
            };
            EGLContext context = eglCreateContext(display, config, NULL, context_attrib_list);

            // Step 8 - Bind the context to the current thread
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);
            mygl::load(reinterpret_cast<mygl::loader_function>(eglGetProcAddress));
            std::string tag = "spdlog-android";
            auto android_logger = spdlog::android_logger_mt("android", tag);
            spdlog::set_default_logger(android_logger);
#else
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            //glfwWindowHint(GLFW_VISIBLE, false);
            const auto w = glfwCreateWindow(1, 1, "_", nullptr, nullptr);
            glfwMakeContextCurrent(w);
            glfwHideWindow(w);
            mygl::load(reinterpret_cast<mygl::loader_function>(glfwGetProcAddress));
#endif

            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback([](GLenum source, GLenum type, std::uint32_t id, GLenum severity, std::int32_t length, const char* message, const void* userParam) {
                switch (type)
                {
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    spdlog::warn("OpenGL Deprecated: {}", message);
                    break;
                case GL_DEBUG_TYPE_ERROR:
                    spdlog::error("OpenGL Error: {}", message);
                    break;
                case GL_DEBUG_TYPE_MARKER:
                    spdlog::info("OpenGL Marker: {}", message);
                    break;
                case GL_DEBUG_TYPE_OTHER:
                    spdlog::debug("OpenGL Other: {}", message);
                    break;
                case GL_DEBUG_TYPE_PERFORMANCE:
                    spdlog::warn("OpenGL Performance: {}", message);
                    break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    spdlog::warn("OpenGL Undefined Behavior: {}", message);
                    break;
                case GL_DEBUG_TYPE_PORTABILITY:
                    spdlog::warn("OpenGL Portability: {}", message);
                    break;
                case GL_DEBUG_TYPE_PUSH_GROUP:
                    spdlog::debug("OpenGL Push Group: {}", message);
                    break;
                case GL_DEBUG_TYPE_POP_GROUP:
                    spdlog::debug("OpenGL Push Group: {}", message);
                    break;
                }
                }, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);

            _processor = std::make_unique<photogrammetry_processor>();
            prom.set_value();
            while (!_quit)
            {
                for (auto& i : std::exchange(_work_items, {}))
                    i();
                while (_work_items.empty() && !_quit) {  // loop to avoid spurious wakeups
                    _proc_wakeup.wait(lock);
                }
            }
            _processor.reset();

#ifdef __ANDROID__
            eglDestroyContext(display, context);
#else
            glfwDestroyWindow(w);
#endif
        });

        fut.wait();
    }
    photogrammetry_processor_async::~photogrammetry_processor_async()
    {
        _quit = true;
        _proc_wakeup.notify_one();
        if (_worker.joinable())
            _worker.join();
    }
    void photogrammetry_processor_async::clear()
    {
        std::unique_lock<std::mutex> lock(_proc_mtx);
        _work_items.emplace_back([this] {
            _processor->clear();
            });
        _proc_wakeup.notify_one();
    }
    void photogrammetry_processor_async::add_image(std::shared_ptr<image> img, float focal_length, std::function<void()> on_finish)
    {
        _enqueued.emplace_back(img, focal_length, on_finish);
    }
    void photogrammetry_processor_async::detect_all()
    {
        std::unique_lock<std::mutex> lock(_proc_mtx);
        _work_items.emplace_back([this, elem = std::exchange(_enqueued, {})]{
            for (auto const& it : elem)
            {
                _processor->add_image(std::get < std::shared_ptr<image>>(it), std::get<float>(it));
                std::get<std::function<void()>>(it)();
            }
            });
        _proc_wakeup.notify_one();
    }
    void photogrammetry_processor_async::match_all()
    {
        std::unique_lock<std::mutex> lock(_proc_mtx);
        _work_items.emplace_back([this] {
            _processor->match_all();
            });
        _proc_wakeup.notify_one();
    }
    photogrammetry_processor& photogrammetry_processor_async::base_processor() { return *_processor; }
    const photogrammetry_processor& photogrammetry_processor_async::base_processor() const { return *_processor; }
}