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

namespace mpp::sift {
    namespace
    {
        constexpr float pi = 3.141592653587f;

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

        struct perf_log
        {
            std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

            void next(const std::string& msg)
            {
                spdlog::info("{} ({} ms)", msg, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_begin).count());
                time_begin = std::chrono::steady_clock::now();
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
            }

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
            glFinish();
        }

        void apply_viewport(int x, int y, int w, int h)
        {
            glViewport(x, y, w, h);
            glScissor(x, y, w, h);
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
                apply_viewport(0, 0, base_width << o, base_height << o);
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
                    apply_viewport(0, 0, base_width >> mip, base_height >> mip);
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.feature_textures[scale], mip);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, state.feature_stencil_buffers[scale + mip * state.num_feature_scales]);

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
        void compute_orientations(detail::sift_state& state, const detection_settings& settings, const std::vector<imgf>& gaussian_images, int x, int y, int octave, int scale, Fun&& publish_orientation)
        {
            const auto& img = gaussian_images[octave * state.difference_of_gaussian_textures.size() + scale];
            const glm::ivec2 px(x >> octave, y >> octave);
            const glm::ivec2 tsize(img.w, img.h);
            constexpr int window_size_half = 5;

            // Discard features where the window does not fit inside the image.
            if (px.x - window_size_half <= 0 || px.x + window_size_half >= tsize.x - 1 || px.y - window_size_half <= 0 || px.y + window_size_half > tsize.y - 1)
                return;

            // Subdivide 360 degrees into 36 bins of 10 degrees.
            // Then compute a gaussian- and magnitude-weighted orientation histogram.
            std::vector<float> angles;
            std::vector<glm::vec2> vectors;
            for (int win_y = -window_size_half; win_y <= window_size_half; ++win_y)
            {
                for (int win_x = -window_size_half; win_x <= window_size_half; ++win_x)
                {
                    int x = px.x + win_x;
                    int y = px.y + win_y;
                    float xdiff = img.read(x + 1, y).r - img.read(x - 1, y).r;
                    float ydiff = img.read(x, y + 1).r - img.read(x, y - 1).r;
                    float mag = std::sqrt(xdiff * xdiff + ydiff * ydiff);

                    float g = gaussian(2.5f, length(glm::vec2(win_x + 0.5f, win_y + 0.5f)));
                    angles.emplace_back(std::atan2(ydiff, xdiff));
                    vectors.emplace_back(g * xdiff, g * ydiff);
                }
            }

            float mag_max = 0;
            float ang_max = pi / 2.f;
            const auto slices = settings.orientation_slices;
            const auto step = (2 * pi) / slices;
            for (int angle = 0; angle < slices; ++angle)
            {
                const float min_angle = step * angle - pi;
                const float max_angle = min_angle + pi / 3.f;

                glm::vec2 sum(0, 0);
                for (size_t i = 0; i < angles.size(); ++i)
                {
                    const auto ang_mod = std::fmodf(angles[i] + 2 * pi, 2 * pi);
                    if (ang_mod >= min_angle && ang_mod <= max_angle)
                    {
                        sum += vectors[i];
                    }
                }

                const auto length2 = dot(sum, sum);
                if (length2 > mag_max)
                {
                    mag_max = length2;
                    ang_max = std::atan2(sum.y, sum.x);
                }
            }
            if (mag_max > settings.orientation_magnitude_threshold)
                publish_orientation(ang_max);
        }

        void build_feature_descriptor(detail::sift_state& state, const imgf& img, int octave, int scale, feature& ft)
        {
            feature::descriptor_t& dc = ft.descriptor;

            // Assume (from previous steps) that the feature must be more than 4 pixels away from the image border.
            using histogram_buckets_t = std::array<std::array<std::array<float, 8>, 4>, 4>; // a 4x4-array of 8-element histograms.
            histogram_buckets_t& bins = reinterpret_cast<histogram_buckets_t&>(dc.histrogram);

            const auto px = int(std::round(ft.x)) >> octave;
            const auto py = int(std::round(ft.y)) >> octave;

            // iterate over frames.
            for (int fy = -2; fy < 2; ++fy)
            {
                for (int fx = -2; fx < 2; ++fx)
                {
                    std::array<float, 8> & bucket = bins[fy + 2][fx + 2];
                    const int fdx = fx * 4;
                    const int fdy = fy * 4;

                    // iterate over frame elements
                    for (int ex = 0; ex < 4; ++ex)
                    {
                        for (int ey = 0; ey < 4; ++ey)
                        {
                            const int x = px + fdx + ex;
                            const int y = py + fdy + ey;
                            const float xdiff = img.read(x + 1, y).r - img.read(x - 1, y).r;
                            const float ydiff = img.read(x, y + 1).r - img.read(x, y - 1).r;
                            const float mag = std::sqrt(xdiff * xdiff + ydiff * ydiff);
                            const float theta = std::atan2(ydiff, xdiff) + pi; // normalize to [0, 2*PI]
                            float g = gaussian(2.5f,
                                length(glm::vec2(ex - 1.5f, ey - 1.5f)));

                            // compute angle difference to dominant orientation. In range rad[0, 2*pi] (deg[0°, 360°])
                            const float angle_diff = std::fmodf((ft.orientation - theta) + 3 * pi, 2 * pi);
                            const int angle_index = int(std::floor((angle_diff / (2 * pi)) * 8));
                            bucket[angle_index] += g * mag;
                        }
                    }
                }
            }
        }
    }

    std::vector<feature> detect_features(const image& img, const detection_settings& settings)
    {
        // Before starting, capture the OpenGL state for a seamless interaction
        opengl_state_capture capture_state;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glDisable(GL_DEPTH_TEST);

        perf_log plog;
        detail::sift_state state(settings.octaves, settings.feature_scales, img.dimensions().x, img.dimensions().y);
        
        // Fill temp[0] with original image
        glBindTexture(GL_TEXTURE_2D, state.temporary_textures[0]);
        constexpr std::array<GLenum, 4> gl_components{ GL_RED, GL_RG, GL_RGB, GL_RGBA };
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img.dimensions().x, img.dimensions().y, gl_components[img.components()-1], GL_UNSIGNED_BYTE, img.data());

        plog.next("(SIFT) Init");

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
        plog.next("(SIFT) Gaussian");

        // STEP 2: Generate Difference-of-Gaussian images (only the full-size ones)
        generate_difference_of_gaussian(state);
        // ... and build pyramid just using mipmaps
        for (auto id : state.difference_of_gaussian_textures)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        plog.next("(SIFT) Difference of Gaussian");

        // STEP 3: Detect feature candidates by testing for extrema
        detect_candidates(state, base_width, base_height);
        plog.next("(SIFT) Detect Candidates");

        // STEP 4: Filter features to exclude outliers and to improve accuracy
        filter_features(state, base_width, base_height);
        plog.next("(SIFT) Filter Features");

        // Download texture data for further processing on the CPU
        std::vector<feature> all_feature_points;
        std::vector<size_t> scale_begins(state.feature_textures.size());
        std::vector<std::vector<glm::vec4>> temp_feature_vectors(state.feature_textures.size() * settings.octaves);
        std::vector<std::vector<feature>> feature_vectors(state.feature_textures.size());
        std::vector<imgf> gaussians(settings.octaves * state.difference_of_gaussian_textures.size());
        for (int scale = 0; scale < state.difference_of_gaussian_textures.size(); ++scale)
        {
            for (int octave = 0; octave < settings.octaves; ++octave)
            {
                glBindTexture(GL_TEXTURE_2D, state.difference_of_gaussian_textures[scale]);
                gaussians[octave * state.difference_of_gaussian_textures.size() + scale].w = base_width >> octave;
                gaussians[octave * state.difference_of_gaussian_textures.size() + scale].h = base_height >> octave;
                gaussians[octave * state.difference_of_gaussian_textures.size() + scale].values.resize((base_width >> octave) * (base_height >> octave));
                int w, h;
                glGetTexLevelParameteriv(GL_TEXTURE_2D, octave, GL_TEXTURE_WIDTH, &w);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, octave, GL_TEXTURE_HEIGHT, &h);

                assert(w == base_width >> octave && h == base_height >> octave);
                glGetTexImage(GL_TEXTURE_2D, octave, GL_RED, GL_FLOAT, gaussians[octave * state.difference_of_gaussian_textures.size() + scale].values.data());
            }
        }
        plog.next("(SIFT) Download Gaussian");

        for (int scale = 0; scale < state.feature_textures.size(); ++scale)
        {
            for (int octave = 0; octave < settings.octaves; ++octave)
            {
                const int out_idx = scale + octave * int(state.feature_textures.size());
                glBindTexture(GL_TEXTURE_2D, state.feature_textures[scale]);
                const auto s = size_t(base_width >> octave) * (base_height >> octave);

                auto& dst = temp_feature_vectors[out_idx];
                dst.resize(s);
                glGetTexImage(GL_TEXTURE_2D, octave, GL_RGBA, GL_FLOAT, dst.data());
            }
        }
        plog.next("(SIFT) Download Features");

        // STEP 5: Compute main orientations at the feature points
        // +
        // STEP 6: Build feature descriptors
        for_n(std::execution::par_unseq, int(state.feature_textures.size()), [&](int scale) {
            auto& fvec = feature_vectors[scale];
            for (int octave = 0; octave < settings.octaves; ++octave)
            {
                auto& src = temp_feature_vectors[scale + octave * int(state.feature_textures.size())];
                for (size_t i = 0; i < src.size(); ++i)
                {
                    auto& tfeat = src[i];
                    if (glm::any(glm::notEqual(tfeat, glm::vec4(0, 0, 0, 1.f))))
                    {
                        // Conditional. Calls the lambda only if orientation magnitude is greater than the threshold.
                        compute_orientations(state, settings, gaussians, int(round(tfeat.x)), int(round(tfeat.y)), octave, int(round(tfeat.z)), [&](float orientation) {
                            fvec.emplace_back(feature{ float(tfeat.x), float(tfeat.y), float(tfeat.z), float(tfeat.w), orientation, octave });
                            });
                    }
                }
            }
            for (auto& feat : fvec)
                build_feature_descriptor(state, gaussians[feat.octave * settings.feature_scales + size_t(std::round(feat.sigma))], feat.octave, size_t(std::round(feat.sigma)), feat);
            });
        plog.next("(SIFT) Compute Orientations and Descriptors");

        // Merge multithreaded results
        for (const auto& x : feature_vectors)
            all_feature_points.insert(all_feature_points.end(), x.begin(), x.end());
        plog.next("(SIFT) Merging Features");

        return all_feature_points;
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

    std::vector<match> match_features(const std::vector<feature>& a, const std::vector<feature>& b, const match_settings& settings)
    {
        std::vector<std::map<float, std::pair<const sift::feature*, const sift::feature*>, std::greater<float>>> amatches(16);

        struct distance_compute
        {
            float operator()(const sift::feature* a, const sift::feature* b) const
            {
                return cosine_similarity(a->descriptor.histrogram.data(), b->descriptor.histrogram.data(), 128);
            }
        } compute;
        perf_log plog;

        std::atomic_int count = 0;

        for_n(std::execution::par_unseq, 16, [&](int i) {
            std::map<float, const sift::feature*, std::greater<float>> vec;
            for (int j = i; j * 16 + i < a.size(); ++j)
            {
                auto& fta = a[j * 16 + i];
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
        plog.next("(SIFT) Compute Matches");

        std::map<float, std::pair<const sift::feature*, const sift::feature*>, std::greater<float>> best_matches;
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
        plog.next("(SIFT) Merging Matches");

        return features;
    }
}
