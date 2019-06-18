#include <impls/gl43_impl.hpp>
#include <processing/sift/sift.hpp>
#include <processing/image.hpp>
#include <fstream>
#include <string>
#include <opengl/mygl_glfw.hpp>
#include <imgui/imgui.h>
#include <platform/opengl.hpp>
#include <map>
#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>
#include <processing/epipolar.hpp>
#include <random>
#include <processing/photogrammetry.hpp>
#include <tinyfd/fd.hpp>

namespace mpp
{
    namespace
    {
        std::uint32_t create_shader(GLenum type, const char* src)
        {
            std::uint32_t sh = glCreateShader(type);
            glShaderSource(sh, 1, &src, nullptr);
            glCompileShader(sh);
            return sh;
        }

        std::uint32_t create_program(std::uint32_t frag, std::uint32_t vert)
        {
            std::uint32_t program = glCreateProgram();
            glAttachShader(program, vert);
            glAttachShader(program, frag);
            glLinkProgram(program);
            int log_len;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len > 3)
            {
                std::string info_log(log_len, '\0');
                glGetProgramInfoLog(program, log_len, &log_len, info_log.data());
                spdlog::info("Shader Compilation Output:\n{}", info_log);
            }
            glDetachShader(program, vert);
            glDetachShader(program, frag);
            return program;
        }

        constexpr auto screen_vert_src = R"(#version 330 core
out vec2 vs_uv;
void main()
{
    gl_Position = vec4(mix(-1.f, 3.f, float(gl_VertexID & 0x1)), mix(-1.f, 3.f, float((gl_VertexID >> 1) & 0x1)), 0.f, 1.f);
    vs_uv = ((gl_Position.xy+1)*0.5f);
}
)";

        constexpr auto texture_frag_scr = R"(#version 330 core
in vec2 vs_uv;
layout(location=0) out vec4 color;
uniform sampler2D in_texture;
void main()
{
    color = texture(in_texture, vec2(vs_uv.x, 1-vs_uv.y));
}
)";
        constexpr auto points_vert_src = R"(#version 330 core
layout(location=0) in vec2 in_pos;
void main()
{
    gl_Position = vec4(in_pos, 0, 1);
}
)";
        constexpr auto simple_color_frag_src = R"(#version 330 core
layout(location=0) out vec4 color;
uniform vec4 u_color;
void main()
{
    color = u_color;
}
)";
    }
    gl43_impl::gl43_impl()
    {
        use_environment<opengl_environment>();
    }

    std::uint32_t allocate_textures(GLenum format, bool gen_mipmap, const image& img)
    {
        std::vector<std::uint32_t> tex(1);
        glGenTextures(int(tex.size()), tex.data());

        for (auto id : tex)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLenum swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, reinterpret_cast<int*>(swizzleMask));
            glTexImage2D(GL_TEXTURE_2D, 0, format, img.dimensions().x, img.dimensions().y, 0, GL_RED, GL_UNSIGNED_BYTE, img.data());
            if (gen_mipmap)
                glGenerateMipmap(GL_TEXTURE_2D);
        }

        return tex[0];
    }

    void gl43_impl::on_setup(program_state& state)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    void gl43_impl::on_start(program_state& state)
    {
        glEnable(GL_MULTISAMPLE);
        const auto screen_vert = create_shader(GL_VERTEX_SHADER, screen_vert_src);
        const auto texture_frag = create_shader(GL_FRAGMENT_SHADER, texture_frag_scr);
        full_screen.program = create_program(texture_frag, screen_vert);
        glDeleteShader(screen_vert);
        glDeleteShader(texture_frag);
        full_screen.in_texture_location = glGetUniformLocation(full_screen.program, "in_texture");

        const auto points_vert = create_shader(GL_VERTEX_SHADER, points_vert_src);
        const auto simple_color_frag = create_shader(GL_FRAGMENT_SHADER, simple_color_frag_src);
        points.program = create_program(simple_color_frag, points_vert);
        points.u_color_location = glGetUniformLocation(points.program, "u_color");
        glDeleteShader(points_vert);
        glDeleteShader(simple_color_frag);

        glGenVertexArrays(1, &empty_vao);
        glGenVertexArrays(1, &points.vao);
        glBindVertexArray(points.vao);
        glGenBuffers(1, &points.vbo);
        glGenBuffers(1, &points.ori_vbo);
        glEnableVertexAttribArray(0);

#if 0
        add_img("../../res/IMG_20190605_174704.jpg");
        add_img("../../res/IMG_20190605_174705.jpg");
#elif 1
        add_img("../../res/IMG_20190616_140852.jpg");
        add_img("../../res/IMG_20190616_140855.jpg");
#elif 1
        add_img("../../res/IMG_20190614_113934.jpg");
        add_img("../../res/IMG_20190614_113954.jpg");
#else
        add_img("../../res/mango/m1.jpg");
        add_img("../../res/mango/m3.jpg");
#endif

        std::shared_ptr<image> imga = std::make_shared<image>();
        std::ifstream file("../../res/IMG_20190616_140852.jpg", std::ios::binary | std::ios::in);
        imga->load_stream(file, 1);
        std::shared_ptr<image> imgb = std::make_shared<image>();
        file = std::ifstream("../../res/IMG_20190616_140855.jpg", std::ios::binary | std::ios::in);
        imgb->load_stream(file, 1);
        std::shared_ptr<image> imgc = std::make_shared<image>();
        file = std::ifstream("../../res/IMG_20190616_140857.jpg", std::ios::binary | std::ios::in);
        imgc->load_stream(file, 1);

        photogrammetry_processor pp;
        pp.add_image(imgc, 0.028f);
        pp.add_image(imgb, 0.028f);
        pp.add_image(imga, 0.028f);
        pp.match_all();
        const auto fh = pp.build_flat_hierarchy();

        spdlog::info("Img 1 -> Img 2: {}", glm::to_string(*pp.relative_matrix(imga, imgb)));

        spdlog::info("Fundamental matrix: {}", glm::to_string(*pp.fundamental_matrix(imga, imgb)));
        spdlog::info("Fundamental matrix: {}", glm::to_string(*pp.fundamental_matrix(imga, imgc)));
        spdlog::info("Fundamental matrix: {}", glm::to_string(*pp.fundamental_matrix(imgb, imgc)));

        sift::match_settings settings;
        settings.relation_threshold = 0.8f;
        settings.similarity_threshold = 0.83f;
        settings.max_match_count = 50;
        auto matches12 = sift::match_features(features[0], features[1], settings);
        auto pts = sift::corresponding_points(matches12);
        glm::mat3 best_mat = ransac_fundamental(pts);
     /*   spdlog::info("Fundamental matrix: {}", glm::to_string(best_mat));
        for (int i = 0; i < pts.size(); ++i)
        {
            auto a = glm::vec3(pts[i].first, 1);
            auto b = glm::vec3(pts[i].second, 1);
            spdlog::info("Test a match:");
            spdlog::info("  A = {}", to_string(a));
            spdlog::info("  B = {}", to_string(b));
            spdlog::info("  F = {}", to_string(best_mat));
            spdlog::info("  B^T * F * A = {}", dot(b, best_mat * a));
        }*/

        for (int i = 0; i < matches12.size(); ++i)
        {
            auto& m = matches12[i];
            ref.emplace_back(glm::vec2(((m.a.x + 1) / 2.f) - 1, m.a.y), glm::vec2(((m.b.x + 1) / 2.f), m.b.y));
        }
    }
    void gl43_impl::on_update(program_state& state, seconds delta)
    {
        double cx, cy;
        glfwGetCursorPos(glfwGetCurrentContext(), &cx, &cy);
        int fx, fy;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &fx, &fy);

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(empty_vao);
        glUseProgram(full_screen.program);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(full_screen.in_texture_location, 0);
        glViewport(0, 0, fx / 2, fy);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(points.vao);
        glUseProgram(points.program);
        glUniform4f(points.u_color_location, 1.f, 0.4f, 0.1f, 1.f);
        glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(sift::feature), nullptr);
        glPointSize(point_size);
        glBufferData(GL_ARRAY_BUFFER, features[0].size() * sizeof(sift::feature), features[0].data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, features[0].size());
        glBindBuffer(GL_ARRAY_BUFFER, points.ori_vbo);
        glLineWidth(1.f);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
        glBufferData(GL_ARRAY_BUFFER, orientation_dbg[0].size() * sizeof(glm::vec2), orientation_dbg[0].data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, orientation_dbg[0].size());

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(empty_vao);
        glUseProgram(full_screen.program);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(full_screen.in_texture_location, 0);
        glViewport(fx / 2, 0, fx / 2, fy);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(points.vao);
        glUseProgram(points.program);
        glUniform4f(points.u_color_location, 1.f, 0.4f, 0.1f, 1.f);
        glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(sift::feature), nullptr);
        glPointSize(point_size);
        glBufferData(GL_ARRAY_BUFFER, features[1].size() * sizeof(sift::feature), features[1].data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, features[1].size());
        glBindBuffer(GL_ARRAY_BUFFER, points.ori_vbo);
        glLineWidth(1.f);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
        glBufferData(GL_ARRAY_BUFFER, orientation_dbg[1].size() * sizeof(glm::vec2), orientation_dbg[1].data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, orientation_dbg[1].size());
        glViewport(0, 0, fx, fy);

        glBindVertexArray(points.vao);
        glUseProgram(points.program);
        glBindBuffer(GL_ARRAY_BUFFER, points.ori_vbo);
        glBufferData(GL_ARRAY_BUFFER, ref.size() * 2 * sizeof(glm::vec2), ref.data(), GL_DYNAMIC_DRAW);

        // Draw Match Lines
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
        glLineWidth(1.f);
        glUniform4f(points.u_color_location, 1.f, 1.f, 1.f, 1.f);
        glDrawArrays(GL_LINES, 0, (num_matches == -1 ? ref.size() : num_matches) * 2);
        // Draw Match Src Points
        glPointSize(4.f);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(glm::vec2), nullptr);
        glUniform4f(points.u_color_location, 0.1f, 0.5f, 1.f, 1.f);
        glDrawArrays(GL_POINTS, 0, (num_matches == -1 ? ref.size() : num_matches));
        // Draw Match Dst Points
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(glm::vec2), reinterpret_cast<void const*>(sizeof(glm::vec2)));
        glUniform4f(points.u_color_location, 0.4f, 0.7f, 1.f, 1.f);
        glDrawArrays(GL_POINTS, 0, (num_matches == -1 ? ref.size() : num_matches));

        if (ImGui::Begin("Settings"))
        {
            ImGui::DragInt("Matches", &num_matches, 0.01f, -1, ref.size());
            ImGui::DragFloat("Point Size", &point_size, 0.01f, 1.f, 100.f);
            ImGui::Text("Cursor at %0.7f, %0.7f", (cx / fx) * 2.f - 1.f, (1 - cy / fy) * 2.f - 1.f);

            if (ImGui::Button("Open..."))
            {
                for (auto& f : open_files("Whatever"))
                {
                    spdlog::info("{}", f.string());
                }
            }

        }
        ImGui::End();
    }
    void gl43_impl::on_end(program_state& state)
    {
        glDeleteVertexArrays(1, &empty_vao);
        glDeleteProgram(full_screen.program);
        glDeleteProgram(points.program);
        glDeleteVertexArrays(1, &points.vao);
        glDeleteBuffers(1, &points.vbo);
        glDeleteTextures(int(textures.size()), textures.data());
        if (_sift_cache)
            _sift_cache.reset();
    }
    void gl43_impl::add_img(const char* path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        img.load_stream(file, 1);
        auto& ori = orientation_dbg.emplace_back();

        constexpr auto max_width = 400;
        const float aspect = img.dimensions().x / img.dimensions().y;
        const auto w = max_width;
        const auto h = aspect * max_width;
        sift::detection_settings settings;
        settings.octaves = 4;
        settings.feature_scales = 3;
        settings.orientation_magnitude_threshold = 0.0002f;
        if (!_sift_cache)
            _sift_cache = sift::create_cache(settings.octaves, settings.feature_scales);

        for (auto& feat : features.emplace_back(sift::detect_features(*_sift_cache, image(img).resize(w, h), settings, sift::dst_system::normalized_coordinates)))
        {
            ori.emplace_back(glm::vec2(feat.x, feat.y));
            ori.emplace_back(glm::vec2(feat.x, feat.y) + 0.05f * glm::vec2(glm::cos(feat.orientation), glm::sin(feat.orientation)));
        }
        textures.emplace_back(allocate_textures(GL_RED, true, img));
    }
}