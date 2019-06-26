#define GLM_LANG_STL11_FORCED
#include "sift.hpp"
#include <processing/sift/detail/sift_state.hpp>
#include <functional>
#include <chrono>
#include <processing/image.hpp>
#include <opengl/mygl.hpp>
#include <array>
#include <map>
#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <atomic>
#include <processing/algorithm.hpp>
#include <spdlog/spdlog.h>
#include <processing/perf_log.hpp>
#include <processing/gl_clock.hpp>
#include <glm/gtx/string_cast.hpp>

namespace mpp::sift {
    namespace
    {
        struct imgf
        {
            int w, h;
            std::vector<float> values;

            glm::vec4 read(int x, int y) const {
                x = std::clamp(x, 0, w - 1);
                y = std::clamp(y, 0, h - 1);
                return glm::vec4(values[x + y * w]);
            }
        };

        struct opengl_state_capture
        {
            opengl_state_capture() {
                glGetIntegerv(GL_ACTIVE_TEXTURE, &active_texture);
                for (int i = 0; i < 4; ++i)
                {
                    glActiveTexture(GLenum(GL_TEXTURE0 + i));
                    glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex2d_bindings[i]);
                }
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
                glGetIntegerv(GL_VIEWPORT, viewport);
                glGetIntegerv(GL_SCISSOR_BOX, scissor);
                glGetIntegerv(GL_DEPTH_TEST, &depth_test_enabled);
                glGetIntegerv(GL_STENCIL_TEST, &stencil_test_enabled);
                glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
                glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
                glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment);
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_framebuffer);
                glGetFloatv(GL_COLOR_CLEAR_VALUE, clear_color);
                glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &clear_stencil);
                glGetIntegerv(GL_STENCIL_FUNC, &stencil.func);
                glGetIntegerv(GL_STENCIL_REF, &stencil.ref);
                glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencil.mask);
                glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &stencil.zfail);
                glGetIntegerv(GL_STENCIL_FAIL, &stencil.fail);
                glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &stencil.pass);
                glGetIntegerv(GL_STENCIL_WRITEMASK, &stencil.wmask);
                glGetIntegerv(GL_RASTERIZER_DISCARD, &rasterizer_discard);
                glGetIntegerv(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, &tf_buffer_binding);
            }
            ~opengl_state_capture() {
                for (int i = 0; i < 4; ++i)
                {
                    glActiveTexture(GLenum(GL_TEXTURE0 + i));
                    glBindTexture(GL_TEXTURE_2D, std::uint32_t(tex2d_bindings[i]));
                }
                glActiveTexture(GLenum(active_texture));
                glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
                glClearStencil(clear_stencil);
                glBindVertexArray(current_vao);

                glStencilFunc(GLenum(stencil.func), stencil.ref, stencil.mask);
                glStencilOp(GLenum(stencil.fail), GLenum(stencil.zfail), GLenum(stencil.pass));
                glStencilMask(stencil.wmask);

                glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
                glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
                glEnable(GL_DEPTH_TEST);
                glUseProgram(current_program);
                glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);
                glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment);
                glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);
                (depth_test_enabled ? glEnable : glDisable)(GL_DEPTH_TEST);
                (rasterizer_discard ? glEnable : glDisable)(GL_RASTERIZER_DISCARD);
                (stencil_test_enabled ? glEnable : glDisable)(GL_STENCIL_TEST);
            }

            int rasterizer_discard;
            int tf_buffer_binding;
            int current_vao;
            int active_texture;
            int tex2d_bindings[4];
            int viewport[4];
            int scissor[4];
            int depth_test_enabled;
            int stencil_test_enabled;
            int current_program;
            int pack_alignment;
            int unpack_alignment;
            int current_framebuffer;
            float clear_color[4];
            int clear_stencil;
            struct {
                int func;
                int ref;
                int mask;
                int fail;
                int zfail;
                int pass;
                int wmask;
            } stencil;
        };

        void dispatch()
        {
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        void apply_gaussian(detail::sift_state & state)
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
            glFinish();
        }

        void apply_viewport(int x, int y, int w, int h)
        {
            glViewport(x, y, w, h);
            glScissor(x, y, w, h);
        }

        void generate_difference_of_gaussian(detail::sift_state & state)
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

        void detect_candidates(detail::sift_state & state, int base_width, int base_height)
        {
            glUseProgram(state.maximize.program);
            glUniform1i(state.maximize.u_previous_tex_location, 0);
            glUniform1i(state.maximize.u_current_tex_location, 1);
            glUniform1i(state.maximize.u_next_tex_location, 2);

            glClearColor(0, 0, 0, 1);
            for (int o = 0; o < state.num_octaves; ++o)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[o]);
                apply_viewport(0, 0, base_width >> o, base_height >> o);
                for (size_t feature_scale = 0; feature_scale < state.feature_textures.size(); ++feature_scale)
                {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, state.feature_textures[feature_scale], o);
                    glClear(GL_COLOR_BUFFER_BIT);
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

        struct tf_feature
        {
            glm::vec4 feat;
            std::int32_t octave;
            float orientation;
            std::int32_t _pad[2];
        };

        int filter_features(detail::sift_state & state, int base_width, int base_height, int samples_passed)
        {
            glUseProgram(state.filter.program);
            glUniform1i(state.filter.u_previous_tex_location, 0);
            glUniform1i(state.filter.u_current_tex_location, 1);
            glUniform1i(state.filter.u_next_tex_location, 2);
            glUniform1i(state.filter.u_feature_tex_location, 3);
            // leave out border of a couple of pixels
            glUniform1i(state.filter.u_border_location, 8);
            glEnable(GL_RASTERIZER_DISCARD);
            std::int64_t tf_buf_offset = 0;
            std::uint32_t tf_query;
            glGenQueries(1, &tf_query);
            glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, state.filter_transform_buffer);
            int buffer_size;
            glGetBufferParameteriv(GL_TRANSFORM_FEEDBACK_BUFFER, GL_BUFFER_SIZE, &buffer_size);
            if (buffer_size < samples_passed * sizeof(tf_feature))
                glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, samples_passed * sizeof(tf_feature), nullptr, GL_DYNAMIC_COPY);

            for (int mip = 0; mip < state.num_octaves; ++mip)
            {
                glUniform1i(state.filter.u_mip_location, mip);
                for (int scale = 0; scale < state.feature_textures.size(); ++scale)
                {
                    glUniform1i(state.filter.u_scale_location, scale);
                    // Bind previous, current and next scale DoG image
                    // update sigma uniform
                    // compute stuff
                    glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[mip]);
                    apply_viewport(0, 0, base_width >> mip, base_height >> mip);
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.feature_textures[scale], mip);
                    glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, state.filter_transform_buffer,
                        tf_buf_offset * sizeof(tf_feature), (samples_passed - tf_buf_offset) * sizeof(tf_feature));

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(scale) + 0]);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(scale) + 1]);
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[std::int64_t(scale) + 2]);
                    glActiveTexture(GL_TEXTURE3);
                    glBindTexture(GL_TEXTURE_2D, state.feature_textures[scale]);

                    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, tf_query);
                    glBeginTransformFeedback(GL_POINTS);
                    glDrawArrays(GL_POINTS, 0, (base_width >> mip) * (base_height >> mip));
                    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
                    glEndTransformFeedback();
                    std::uint32_t prims_written = 0;
                    glGetQueryObjectuiv(tf_query, GL_QUERY_RESULT, &prims_written);
                    tf_buf_offset += std::int64_t(prims_written);
                }
            }
            glDisable(GL_RASTERIZER_DISCARD);
            return tf_buf_offset;
        }

        int compute_orientations(detail::sift_state & state, int num_features, int base_width, int base_height) {

            glUseProgram(state.orientation.program);
            for (int i = 0; i < state.difference_of_gaussian_textures.size(); ++i)
            {
                glUniform1i(state.orientation.u_textures_locations[i], i);
                glActiveTexture(GLenum(int(GL_TEXTURE0) + i));
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[i]);
            }

            glEnable(GL_RASTERIZER_DISCARD);
            std::uint32_t tf_query;
            glGenQueries(1, &tf_query);
            int filter_buffer_size = num_features * sizeof(tf_feature);
            int orientation_buffer_size = 0;
            glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, state.orientation_transform_buffer);
            glGetBufferParameteriv(GL_TRANSFORM_FEEDBACK_BUFFER, GL_BUFFER_SIZE, &orientation_buffer_size);
            if (filter_buffer_size > orientation_buffer_size)
                glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, filter_buffer_size, nullptr, GL_DYNAMIC_COPY);

            glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[0]);
            apply_viewport(0, 0, base_width, base_height);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.feature_textures[0], 0);
            glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, state.orientation_transform_buffer,
                0, filter_buffer_size);

            glBindVertexArray(state.orientation_vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, state.filter_transform_buffer);
            glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(tf_feature), (const void*)offsetof(tf_feature, feat));
            glVertexAttribPointer(1, 1, GL_INT, false, sizeof(tf_feature), (const void*)offsetof(tf_feature, octave));

            glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, tf_query);
            glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, num_features);
            glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
            glEndTransformFeedback();
            std::uint32_t prims_written = 0;
            glGetQueryObjectuiv(tf_query, GL_QUERY_RESULT, &prims_written);
            std::int64_t tf_buf_offset = std::int64_t(prims_written);
            glBindVertexArray(0);
            glDisable(GL_RASTERIZER_DISCARD);
            return tf_buf_offset;
        }

        void compute_descriptors(detail::sift_state& state, int num_features)
        {
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, state.orientation_transform_buffer, 0, num_features * sizeof(tf_feature));
            int full_buffer_size = 0;
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, state.full_feature_buffer);
            glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &full_buffer_size);
            if (full_buffer_size < num_features * sizeof(feature))
                glBufferData(GL_SHADER_STORAGE_BUFFER, num_features * sizeof(feature), nullptr, GL_DYNAMIC_COPY);
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, state.full_feature_buffer, 0, num_features * sizeof(feature));
            glUseProgram(state.descriptor.program);

            for (int i = 0; i < state.difference_of_gaussian_textures.size(); ++i)
            {
                glUniform1i(state.descriptor.u_textures_locations[i], i);
                glActiveTexture(GLenum(int(GL_TEXTURE0) + i));
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[i]);
            }
            glDispatchCompute((num_features + 31) / 32, 1, 1);
        }
    }

    struct sift_cache
    {
        sift_cache(size_t num_octaves, size_t num_feature_scales) : state(num_octaves, num_feature_scales) {}
        detail::sift_state state;
    };
    std::shared_ptr<sift_cache> create_cache(size_t num_octaves, size_t num_feature_scales)
    {
        return std::make_shared<sift_cache>(num_octaves, num_feature_scales);
    }

    std::vector<feature> detect_features(const image & img, const detection_settings & settings, dst_system system)
    {
        auto in_state = create_cache(settings.octaves, settings.feature_scales);
        return detect_features(*in_state, img, settings, system);
    }

    std::vector<feature> detect_features(sift_cache & cache, const image & img, const detection_settings & settings, dst_system system)
    {
        using clock_type =
#if defined(__ANDROID__)
            std::chrono::system_clock;
#else
            gl_query_clock;
#endif

        basic_perf_log<std::chrono::microseconds, clock_type> plog("SIFT");

        plog.start();
        // Before starting, capture the OpenGL state for a seamless interaction
        opengl_state_capture capture_state;
        glDisable(GL_MULTISAMPLE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glDisable(GL_DEPTH_TEST);
        auto& state = cache.state;
        state.resize(img.dimensions().x, img.dimensions().y);
        std::vector<float> imgf_data(img.dimensions().x * img.dimensions().y);
        float* ins = imgf_data.data();
        for (auto it = img.data(); it < img.data() + img.size(); it += img.components(), ++ins)
        {
            if (img.components() >= 3)
            {
                const auto& v3 = reinterpret_cast<glm::u8vec3 const&>(*it);
                const auto r = glm::vec3(v3) / 255.f;
                *ins = float(0.21 * r.r + 0.72 * r.g + 0.07 * r.b);
            }
            else
            {
                *ins = float(*reinterpret_cast<const unsigned char*>(it)) / 255.f;
            }
        }

        // Fill temp[0] with original image
        glBindTexture(GL_TEXTURE_2D, state.temporary_textures[0]);
        constexpr std::array<GLenum, 4> gl_components{ GL_RED, GL_RG, GL_RGB, GL_RGBA };
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img.dimensions().x, img.dimensions().y, GL_RED, GL_FLOAT, imgf_data.data());
        plog.step("Initialize prerequisites");

        const int base_width = img.dimensions().x;
        const int base_height = img.dimensions().y;
        apply_viewport(0, 0, base_width, base_height);
        // Use universal empty vertex array
        glBindVertexArray(state.empty_vao);
        // STEP 1: Generate gauss-blurred images
        apply_gaussian(state);

        // ... and build pyramid just using mipmaps
        for (auto id : state.gaussian_textures)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        plog.step("Generate gauss-blurred images");

        // STEP 2: Generate Difference-of-Gaussian images (only the full-size ones)
        generate_difference_of_gaussian(state);

        // ... and build pyramid just using mipmaps
        for (auto id : state.difference_of_gaussian_textures)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        plog.step("Generate Difference-of-Gaussian images (only the full-size ones)");

#ifndef __ANDROID__
        std::uint32_t count_query;
        glGenQueries(1, &count_query);
        glBeginQuery(GL_SAMPLES_PASSED, count_query);
#endif
        // STEP 3: Detect feature candidates by testing for extrema
        detect_candidates(state, base_width, base_height);
        plog.step("Detect feature candidates by testing for extrema");
        constexpr auto max_features = 1u << 14;
        std::uint32_t samples_passed = max_features;
#ifndef __ANDROID__
        glEndQuery(GL_SAMPLES_PASSED);
        glGetQueryObjectuiv(count_query, GL_QUERY_RESULT, &samples_passed);
        glDeleteQueries(1, &count_query);
#endif

        // STEP 4: Filter features to exclude outliers and to improve accuracy
        int num_features = filter_features(state, base_width, base_height, samples_passed);
        plog.step("Filter features to exclude outliers and to improve accuracy");

        num_features = compute_orientations(state, num_features, base_width, base_height);
        plog.step("Orientation Computation");

        compute_descriptors(state, num_features);
        plog.step("Descriptor Computation");

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, state.full_feature_buffer);
        auto d = static_cast<const feature*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, num_features * sizeof(feature), GL_MAP_READ_BIT));
        std::vector<feature> tf_data(d, d + num_features);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        plog.step("Descriptor Download");

        if (system == dst_system::normalized_coordinates)
        {
            std::for_each(tf_data.begin(), tf_data.end(), [&](feature & feat) {
                feat.x = (2.f * feat.x / img.dimensions().x) - 1.f;
                feat.y = -((2.f * feat.y / img.dimensions().y) - 1.f);
                });
            plog.step("Convert to normalized coordinates");
        }
        else if (system == dst_system::image_coordinates)
        {
            std::for_each(tf_data.begin(), tf_data.end(), [&](feature & feat) {
                feat.x = feat.x / img.dimensions().x;
                feat.y = feat.y / img.dimensions().y;
                });
            plog.step("Convert to image coordinates");
        }
        return tf_data;
    }

    float cosine_similarity(const float* a, const float* b, unsigned int size)
    {
        float dot = 0.f;
        float denom_a = 0.f;
        float denom_b = 0.f;
        for (auto i = 0u; i < size; ++i) {
            dot += a[i] * b[i];
            denom_a += a[i] * a[i];
            denom_b += b[i] * b[i];
        }
        return dot / (sqrt(denom_a) * sqrt(denom_b));
    }

    std::vector<match> match_features(const std::vector<feature> & a, const std::vector<feature> & b, const match_settings & settings)
    {
        perf_log plog("SIFT Match");
        plog.start();
        constexpr auto stride = 16;
        std::array<std::multimap<float, std::pair<const sift::feature*, const sift::feature*>, std::greater<float>>, stride> amatches;

        struct distance_compute
        {
            float operator()(const sift::feature* a, const sift::feature* b) const
            {
                return cosine_similarity(a->descriptor.histrogram.data(), b->descriptor.histrogram.data(), 128);
            }
        } compute;

        std::atomic_int count = 0;

        for_n(stride, [&](int i) {
            const int step = (a.size() + stride - 1) / float(stride);
            std::multimap<float, const sift::feature*, std::greater<float>> vec;
            for (int j = i * step; j < (i+1) * step && j < a.size(); j++)
            {
                auto& fta = a[j];
                auto& map = amatches[i];
                vec.clear();
                for (auto& ftb : b)
                {
                    const auto sim = compute(&fta, &ftb);
                    vec.emplace(sim, &ftb);
                }

                auto first = vec.begin();
                auto second = first;
                second++;
                if (!(vec.size() < 2 || second->first / first->first > settings.relation_threshold || first->first < settings.similarity_threshold))
                {
                    map.emplace(first->first, std::make_pair(&fta, first->second));
                    ++count;
                }
            }
            });
        plog.step("Compute matches by finding each features nearest neighbour");

        std::multimap<float, std::pair<const sift::feature*, const sift::feature*>, std::greater<float>> best_matches;
        for (auto const& map : amatches)
        {
            for (const auto& item : map)
                best_matches.emplace(item.first, item.second);
        }

        std::vector<match> features;
        features.reserve(std::min(count.load(), settings.max_match_count));
        auto it = best_matches.begin();
        for (int i = 0; i < std::min(count.load(), settings.max_match_count); ++i)
        {
            features.emplace_back(match{ *it->second.first, *it->second.second, it->first });
            it++;
        }
        plog.step("Merging multithreaded match results.");

        return features;
    }
    std::vector<std::pair<glm::vec2, glm::vec2>> corresponding_points(const std::vector<match> & matches)
    {
        std::vector<std::pair<glm::vec2, glm::vec2>> pt(matches.size());
        size_t i = 0;
        for (auto& m : matches)
            pt[i++] = std::make_pair(glm::vec2(m.a.x, m.a.y), glm::vec2(m.b.x, m.b.y));
        return pt;
    }
}
