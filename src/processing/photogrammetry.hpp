#pragma once
#include <processing/image.hpp>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <optional>
#include <glm/glm.hpp>
#include <processing/sift/sift.hpp>
#include <mutex>
#include <thread>
#include <queue>
#include <atomic>
#include <opengl/mygl_glfw.hpp>

namespace mpp::sift
{
    struct sift_cache;
}

namespace mpp
{
    class photogrammetry_processor
    {
    public:
        struct transformed_image
        {
            std::shared_ptr<image> img;
            glm::mat4 transformation;
        };

        photogrammetry_processor();
        
        void clear();
        void add_image(std::shared_ptr<image> img, float focal_length);
        void match_all();

        sift::detection_settings& detection_settings() noexcept { return _detection_settings; }
        sift::match_settings& match_settings() noexcept { return _match_settings; }

        std::optional<glm::mat3> fundamental_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);
        std::optional<glm::mat3> essential_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);
        std::optional<glm::mat4> relative_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);
        std::vector<transformed_image> build_flat_hierarchy();

    private:
        template<typename Visitor>
        void visit_match_tree_impl(Visitor&& vis, const std::shared_ptr<image>& img, glm::mat4 tf, std::unordered_set<image*>& visited)
        {
            if (const auto it = visited.find(img.get()); it == visited.end())
            {
                vis(img, tf);
                visited.emplace(img.get());
                for (const auto& cp : _image_matches[img])
                {
                    const auto rel = *relative_matrix(img, cp.first);
                    visit_match_tree_impl<Visitor>(std::forward<Visitor>(vis), cp.first, rel * tf, visited);
                }
            }
        }
        template<typename Visitor>
        void visit_match_tree(Visitor&& vis)
        {
            // Find the image with the most correspondences.
            auto largest = _image_matches.begin();
            size_t lsize = 0;
            for (auto it = _image_matches.begin(); it != _image_matches.end(); ++it)
            {
                if (it->second.size() > lsize)
                {
                    largest = it;
                    lsize = it->second.size();
                }
            }

            std::unordered_set<image*> visited;
            vis(largest->first, glm::mat4(1.f));
            visited.emplace(largest->first.get());
            for (const auto& cp : _image_matches[largest->first])
            {
                const auto rel = *relative_matrix(largest->first, cp.first);
                visit_match_tree_impl<Visitor>(std::forward<Visitor>(vis), cp.first, rel, visited);
            }
        }

        sift::detection_settings _detection_settings;
        sift::match_settings _match_settings;
        std::shared_ptr<sift::sift_cache> _sift_cache;

        struct image_info
        {
            std::vector<sift::feature> feature_points;
            glm::mat3 camera_intrinsics;
        };
        std::unordered_map<std::shared_ptr<image>, image_info> _images;

        struct match_list
        {
            glm::mat3 fundamental_matrix;
            std::vector<std::pair<glm::vec2, glm::vec2>> match_points;
        };
        std::unordered_map<std::shared_ptr<image>, std::unordered_map<std::shared_ptr<image>, match_list>> _image_matches;
    };

    class photogrammetry_processor_async
    {
    public:
        photogrammetry_processor_async()
        {
            _worker = std::thread([this] {
                std::unique_lock<std::mutex> lock(_proc_mtx);
                glfwDefaultWindowHints();
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                //glfwWindowHint(GLFW_VISIBLE, false);
                const auto w = glfwCreateWindow(1, 1, "_", nullptr, nullptr);
                glfwMakeContextCurrent(w);
                glfwHideWindow(w);
                mygl::load(reinterpret_cast<mygl::loader_function>(glfwGetProcAddress));

                _processor = std::make_unique<photogrammetry_processor>();
                while (!_quit)
                {
                    while (_work_items.empty()) {  // loop to avoid spurious wakeups
                        _proc_wakeup.wait(lock);
                    }
                    for (auto& i : std::exchange(_work_items, {}))
                        i();
                }

                glfwDestroyWindow(w);
                });
        }
        ~photogrammetry_processor_async()
        {
            _quit = true;
            if (_worker.joinable())
                _worker.join();
        }
        void clear()
        {
            std::unique_lock<std::mutex> lock(_proc_mtx);
            _work_items.emplace_back([this] {
                _processor->clear();
                });
            _proc_wakeup.notify_one();
        }
        void add_image(std::shared_ptr<image> img, float focal_length, std::function<void()> on_finish)
        {
            _enqueued.emplace_back(img, focal_length, on_finish);
        }
        void detect_all()
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
        void match_all()
        {
            std::unique_lock<std::mutex> lock(_proc_mtx);
            _work_items.emplace_back([this] {
                _processor->match_all();
                });
            _proc_wakeup.notify_one();
        }

        photogrammetry_processor& base_processor() { return *_processor; }
        const photogrammetry_processor& base_processor() const { return *_processor; }

    private:
        std::vector<std::tuple<std::shared_ptr<image>, float, std::function<void()>>> _enqueued;
        std::atomic_bool _quit = false;
        std::unique_ptr<photogrammetry_processor> _processor;
        std::mutex _proc_mtx;
        std::condition_variable _proc_wakeup;
        std::thread _worker;
        std::vector<std::function<void()>> _work_items;
    };
}