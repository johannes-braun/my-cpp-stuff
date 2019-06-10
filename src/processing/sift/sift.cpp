#include "sift.hpp"
#include <processing/sift/detail/sift_state.hpp>
#include <functional>
#include <chrono>
#include <iostream>
#include <processing/image.hpp>
#include <opengl/mygl.hpp>
#include <array>

namespace mpp::sift {
    struct perf_log
    {
        std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

        void next(const std::string& msg)
        {
            std::cout << "STEP: [" << msg << "] -- " <<
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_begin).count()
                << "ms\n";
            time_begin = std::chrono::steady_clock::now();
        }
    };

    GLenum get_gl_fmt(const image& img) {
        switch (img.components())
        {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_INVALID_VALUE;
        }
    }

    struct opengl_state_capture
    {
        opengl_state_capture() {
            glGetIntegerv(GL_ACTIVE_TEXTURE, &active_texture);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex2d_binding);
            glGetIntegerv(GL_VIEWPORT, viewport);
            glGetIntegerv(GL_DEPTH_TEST, &depth_test_enabled);
            glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        }
        ~opengl_state_capture() {
            glActiveTexture(GLenum(active_texture));
            glBindTexture(GL_TEXTURE_2D, std::uint32_t(tex2d_binding));
            glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
            glEnable(GL_DEPTH_TEST);
            glUseProgram(current_program);
        }

        int active_texture;
        int tex2d_binding;
        int viewport[4];
        int depth_test_enabled;
        int current_program;
    };

    void dispatch()
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void apply_gaussian(detail::sift_state& state)
    {
        glUseProgram(state.gauss_blur.program);
        glUniform1i(state.gauss_blur.in_texture_location, 0);
        glActiveTexture(GL_TEXTURE0);
        for (int scale = 0; scale < int(state.gaussian_textures.size()); ++scale)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[0]);

            // Write from temp_textures[0] (original) to temp_textures[1] using horizontal blur
            glBindTexture(GL_TEXTURE_2D, state.temporary_textures[0]);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.temporary_textures[1], 0);
            glUniform1i(state.gauss_blur.u_mip_location, 0);
            glUniform1i(state.gauss_blur.u_dir_location, 0);
            glUniform1f(state.gauss_blur.u_sigma_location, std::powf(std::sqrtf(2), float(scale + 1)) * 1.3f);
            dispatch();

            // Write from temp_textures[1] to textures[scale] using vertical blur
            glBindTexture(GL_TEXTURE_2D, state.temporary_textures[1]);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.gaussian_textures[scale], 0);
            glUniform1i(state.gauss_blur.u_dir_location, 1);
            dispatch();
        }
    }

    void generate_difference_of_gaussian(detail::sift_state& state)
    {
        glUseProgram(state.difference.program);
        glUniform1i(state.difference.u_current_tex_location, 0);
        glUniform1i(state.difference.u_previous_tex_location, 1);

        glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[0]);
        // Now compute difference of gaussian_textures[scale] to previous scale from gaussian_textures[scale-1]...
        for (int scale = 1; scale < int(state.gaussian_textures.size()); ++scale)
        {
            // Write it to difference_of_gaussian_textures[scale]
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.difference_of_gaussian_textures[scale - 1], 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, state.gaussian_textures[scale]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, state.gaussian_textures[std::int64_t(scale) - 1]);
            dispatch();
        }
    }

    void detect_candidates(detail::sift_state& state, int base_width, int base_height)
    {
        glUseProgram(state.maximize.program);
        glUniform1i(state.maximize.u_previous_tex_location, 0);
        glUniform1i(state.maximize.u_current_tex_location, 1);
        glUniform1i(state.maximize.u_next_tex_location, 2);

        glClearColor(0, 0, 0, 1);
        glClearStencil(0x0);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xff);
        glStencilOp(GL_ZERO, GL_REPLACE, GL_REPLACE);
        glStencilMask(0xff);

        for (int o = 0; o < state.num_octaves; ++o)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[o]);
            glViewport(0, 0, base_width << o, base_height << o);
            for (size_t feature_scale = 0; feature_scale < state.feature_textures.size(); ++feature_scale)
            {
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.feature_textures[feature_scale], o);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, state.feature_stencil_buffers[feature_scale + o * state.num_feature_scales]);

                const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

                glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(feature_scale)]);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(feature_scale) + 1]);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(feature_scale) + 2]);
                glUniform1i(state.maximize.u_neighbors_location, 0);
                glUniform1i(state.maximize.u_mip_location, o);
                glUniform1i(state.maximize.u_scale_location, int(feature_scale));
                dispatch();
            }
        }
    }

    void filter_features(detail::sift_state& state, int base_width, int base_height)
    {
        glStencilFunc(GL_EQUAL, 1, 0xff);
        glStencilOp(GL_ZERO, GL_REPLACE, GL_REPLACE);
        glStencilMask(0xff);

        glUseProgram(state.filter.program);
        glUniform1i(state.filter.u_previous_tex_location, 0);
        glUniform1i(state.filter.u_current_tex_location, 1);
        glUniform1i(state.filter.u_next_tex_location, 2);
        glUniform1i(state.filter.u_feature_tex_location, 3);
        // leave out border of a couple of pixels
        glUniform1i(state.filter.u_border_location, 8);
        for (int mip = 0; mip < state.num_octaves; ++mip)
        {
            glUniform1i(state.filter.u_mip_location, mip);
            for (int scale = 0; scale < state.feature_textures.size(); ++scale)
            {
                glUniform1i(state.filter.u_scale_location, scale);
                // Bind previous, current and next scale DoG image
                // set current feature image mip as stencil buffer
                // update sigma uniform
                // compute stuff
                glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[mip]);
                glViewport(0, 0, base_width >> mip, base_height >> mip);
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.feature_textures[scale], mip);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, state.feature_stencil_buffers[scale + mip * state.num_feature_scales]);

                auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(scale) + 0]);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(scale) + 1]);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(scale) + 2]);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, state.feature_textures[scale]);
                dispatch();
            }
        }
    }

    float gaussian(float sigma, float diff)
    {
        const float sqrt_2_pi = 2.50662827463f;
        float inner = diff / sigma;
        float nom = exp(-(inner * inner / 2));
        return nom / (sigma * sqrt_2_pi);
    }

    template<typename Fun>
    void compute_orientations(detail::sift_state& state, const std::vector<image>& gaussian_images, int x, int y, int scale, int octave, Fun&& publish_orientation)
    {
        const image& img = gaussian_images[octave * state.gaussian_textures.size() + scale];
        const glm::ivec2 px(x, y);
        const glm::ivec2 tsize = img.dimensions();
        const int window_size_half = std::max(3, 8 >> octave);

        // Discard features where the window does not fit inside the image.
        if (px.x - window_size_half < 0 || px.x + window_size_half >= tsize.x || px.y - window_size_half < 0 || px.y + window_size_half > tsize.y)
        {
            return;
        }

        // Subdivide 360 degrees into 36 bins of 10 degrees.
        // Then compute a gaussian- and magnitude-weighted orientation histogram.
        std::array<float, 36> ori_bins{  };
        constexpr float pi = 3.141592653587f;
        for (int win_y = -window_size_half; win_y < window_size_half; ++win_y)
        {
            for (int win_x = -window_size_half; win_x < window_size_half; ++win_x)
            {
                int x = px.x + win_x;
                int y = px.y + win_y;
                float xdiff = img.read(x + 1, y).r - img.read(x - 1, y).r;
                float ydiff = img.read(x, y + 1).r - img.read(x, y - 1).r;
                float mag = std::sqrt(xdiff * xdiff + ydiff * ydiff);
                float theta = std::atan2(ydiff, xdiff) + pi; // normalize to [0, 2*PI]
                int index = int((theta / (2 * pi)) * 36) % 36;

                float g = gaussian(float(scale + 1) * 1.5f, length(glm::vec2(win_x, win_y)));
                ori_bins[index] += g * mag;
            }
        }

        // Now compute dominant directions
        int cur = 0;
        while (cur < 8)
        {
            int max_id = -1;
            float cmax = -1;
            for (int i = 0; i < 36; ++i)
            {
                if (ori_bins[i] > cmax)
                    cmax = ori_bins[max_id = i];
            }
            if (max_id == -1 || cmax / float(cur) < 0.2f)
            {
                break;
            }

            publish_orientation(float(max_id) * 2.f * pi / 36.f);
            ori_bins[max_id] = 0;

            ++cur;
        }
    }

    std::vector<feature> detect_features(const image& img, size_t octaves, size_t feature_scales)
    {
        // Before starting, capture the OpenGL state for a seamless interaction
        opengl_state_capture capture_state;
        glDisable(GL_DEPTH_TEST);

        perf_log plog;
        detail::sift_state state(octaves, feature_scales, img.dimensions().x, img.dimensions().y);

        // Fill temp[0] with original image
        glBindTexture(GL_TEXTURE_2D, state.temporary_textures[0]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img.dimensions().x, img.dimensions().y, get_gl_fmt(img), GL_UNSIGNED_BYTE, img.data());

        plog.next("Init");

        const int base_width = img.dimensions().x;
        const int base_height = img.dimensions().y;
        glViewport(0, 0, base_width, base_height);

        // STEP 1: Generate gauss-blurred images
        apply_gaussian(state);
        // ... and build pyramid just using mipmaps
        for (auto id : state.gaussian_textures)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        plog.next("Gaussian");

        // STEP 2: Generate Difference-of-Gaussian images (only the full-size ones)
        generate_difference_of_gaussian(state);
        // ... and build pyramid just using mipmaps
        for (auto id : state.difference_of_gaussian_textures)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        plog.next("Difference of Gaussian");

        // STEP 3: Detect feature candidates by testing for extrema
        detect_candidates(state, base_width, base_height);
        plog.next("Detect Candidates");

        // STEP 4: Filter features to exclude outliers and to improve accuracy
        filter_features(state, base_width, base_height);
        plog.next("Filter Features");

        // Download texture data for further processing on the CPU
        std::vector<std::vector<feature>> feature_points(octaves);
        std::vector<feature> all_feature_points;
        std::vector<glm::vec4> feature_vectors;
        std::vector<image> gaussians(octaves * state.gaussian_textures.size());
        for (int scale = 0; scale < state.gaussian_textures.size(); ++scale)
        {
            for (int octave = 0; octave < octaves; ++octave)
            {
                gaussians[octave * state.gaussian_textures.size() + scale].load_empty(base_width >> octave, base_height >> octave, 1);
                glBindTexture(GL_TEXTURE_2D, state.gaussian_textures[scale]);
                glGetTexImage(GL_TEXTURE_2D, octave, GL_RED, GL_UNSIGNED_BYTE, gaussians[octave * state.gaussian_textures.size() + scale].data());
            }
        }
        plog.next("Download Gaussian");

        for (int scale = 0; scale < state.feature_textures.size(); ++scale)
        {
            for (int octave = 0; octave < octaves; ++octave)
            {
                const int out_idx = scale + octave * int(state.feature_textures.size());
                glBindTexture(GL_TEXTURE_2D, state.feature_textures[scale]);
                const auto s = size_t(base_width >> octave) * (base_height >> octave);

                if(feature_vectors.size() < s)
                    feature_vectors.resize(s);
                glGetTexImage(GL_TEXTURE_2D, octave, GL_RGBA, GL_FLOAT, feature_vectors.data());

                for (size_t i = 0; i < s; ++i)
                {
                    auto& feat = feature_vectors[i];
                    if (glm::any(glm::notEqual(feat, glm::vec4(0, 0, 0, 1.f))))
                    {
                        const auto publish_orientation = [&](float orientation) {
                            feature_points[octave].push_back(feature{ float(feat.x), float(feat.y), float(feat.z), float(feat.w), orientation });
                            all_feature_points.emplace_back(feature{ float(feat.x), float(feat.y), float(feat.z), float(feat.w), orientation });
                        };
                        compute_orientations(state, gaussians, 
                            int(std::round(feat.x)) >> octave, int(std::round(feat.y)) >> octave, 
                            int(std::round(feat.z)), octave, publish_orientation);
                    }
                }
            }
        }

        plog.next("Download Features");
        glDisable(GL_STENCIL_TEST);
        return all_feature_points;
    }
}
