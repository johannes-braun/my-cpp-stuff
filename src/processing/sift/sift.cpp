#include "sift.hpp"
#include <processing/sift/detail/sift_state.hpp>
#include <functional>
#include <chrono>
#include <iostream>
#include <processing/image.hpp>
#include <opengl/mygl.hpp>
#include <array>
#include <map>
#include <glm/gtx/hash.hpp>
#include <unordered_set>

namespace mpp::sift {
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
            std::cout << "STEP: [" << msg << "] -- " <<
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_begin).count()
                << "ms\n";
            time_begin = std::chrono::steady_clock::now();
        }
    };

    GLenum get_gl_fmt(const image & img) {
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

    void filter_features(detail::sift_state & state, int base_width, int base_height)
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
    void compute_orientations(detail::sift_state & state, const detection_settings& settings, /*int base_width, int base_height*/ const std::vector<imgf>& gaussian_images, int x, int y, int octave, int scale, Fun&& publish_orientation)
    {
        //glDisable(GL_STENCIL_TEST);
        //glUseProgram(state.orientation.program);
        //for(int i=0; i<state.feature_textures.size(); ++i)
        //    glUniform1i(state.orientation.u_img_location + i, i);
        //glUniform1f(state.orientation.u_orientation_magnitude_threshold_location, settings.orientation_magnitude_threshold);
        //glUniform1i(state.orientation.u_orientation_slices_location, settings.orientation_slices);

        //// leave out border of a couple of pixels
        //for (int mip = 0; mip < state.num_octaves; ++mip)
        //{
        //    glUniform1i(state.orientation.u_mip_location, mip);
        //    for (int i = 0; i < state.feature_textures.size(); ++i)
        //    {
        //        glActiveTexture(GLenum(GL_TEXTURE0 + i));
        //        glBindTexture(GL_TEXTURE_2D, state.feature_textures[i]);
        //    }
        //    for (int scale = 0; scale < state.feature_textures.size(); ++scale)
        //    {
        //        glUniform1i(state.orientation.u_scale_location, scale);
        //        // Bind previous, current and next scale DoG image
        //        // set current feature image mip as stencil buffer
        //        // update sigma uniform
        //        // compute stuff
        //        glBindFramebuffer(GL_FRAMEBUFFER, state.framebuffers[mip]);
        //        glViewport(0, 0, base_width >> mip, base_height >> mip);
        //        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.orientation_textures[scale], mip);
        //        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, state.feature_stencil_buffers[scale + mip * state.num_feature_scales]);

        //        dispatch();
        //    }
        //}

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
        float ang_max = pi/2.f;
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

    void build_feature_descriptor(detail::sift_state & state, const imgf& img, int octave, int scale, feature & ft)
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
                std::array<float, 8>& bucket = bins[fy + 2][fx + 2];
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
                        const float angle_diff = std::fmodf((ft.orientation - theta)+3 * pi, 2 * pi);
                        const int angle_index = int(std::floor((angle_diff / (2 * pi)) * 8));
                        bucket[angle_index] += g * mag;
                    }
                }
            }
        }
    }

    template<typename InputIterator, typename OutputIterator, typename Predicate, typename TransformFunc>
    OutputIterator transform_if(
        InputIterator&& begin,
        InputIterator&& end,
        OutputIterator&& out,
        Predicate&& predicate,
        TransformFunc&& transformer
    ) {
        for (; begin != end; ++begin, ++out) {
            if (predicate(*begin))
                * out = transformer(*begin);
        }
        return out;
    }

    std::vector<feature> detect_features(const image & img, const detection_settings& settings)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        // Before starting, capture the OpenGL state for a seamless interaction
        opengl_state_capture capture_state;
        glDisable(GL_DEPTH_TEST);

        perf_log plog;
        detail::sift_state state(settings.octaves, settings.feature_scales, img.dimensions().x, img.dimensions().y);

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

        /*compute_orientations(state, settings, base_width, base_height);
        plog.next("Compute Orientations");*/

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
        plog.next("Download Gaussian");

        int k = 0;

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
                /*for (size_t i = 0; i < dst.size(); ++i)
                {
                    auto& tfeat = dst[i];
                    if (glm::any(glm::notEqual(tfeat, glm::vec4(0, 0, 0, 1.f))))
                    {
                        compute_orientations(state, settings, gaussians, int(round(tfeat.x)), int(round(tfeat.y)), octave, int(round(tfeat.z)), [&](float orientation) {
                            feature_vectors[scale].emplace_back(feature{ float(tfeat.x), float(tfeat.y), float(tfeat.z), float(tfeat.w), orientation, octave });
                            });
                    }
                }*/
            }
        }
        plog.next("Download Features");
        int z = 0;
#pragma omp parallel for schedule(dynamic)
        for (int scale = 0; scale < state.feature_textures.size(); ++scale)
        {
            for (int octave = 0; octave < settings.octaves; ++octave)
            {
                //const auto dst_idx = octave * state.feature_textures.size() + scale;
                auto& src = temp_feature_vectors[scale + octave * int(state.feature_textures.size())];
                for (size_t i = 0; i < src.size(); ++i)
                {
                    auto& tfeat = src[i];
                    if (glm::any(glm::notEqual(tfeat, glm::vec4(0, 0, 0, 1.f))))
                    {
                        compute_orientations(state, settings, gaussians, int(round(tfeat.x)), int(round(tfeat.y)), octave, int(round(tfeat.z)), [&](float orientation) {
                            feature_vectors[scale].emplace_back(feature{ float(tfeat.x), float(tfeat.y), float(tfeat.z), float(tfeat.w), orientation, octave });
                            });
                    }
                }
            }
        }
        plog.next("Compute Orientations");

#pragma omp parallel for
        for(int i=0; i<feature_vectors.size(); ++i)
            for(auto& feat : feature_vectors[i])
                build_feature_descriptor(state, gaussians[feat.octave * settings.feature_scales + size_t(std::round(feat.sigma))], feat.octave, size_t(std::round(feat.sigma)), feat);
        plog.next("Compute Descriptors");

        for (const auto& x : feature_vectors)
            all_feature_points.insert(all_feature_points.end(), x.begin(), x.end());
        plog.next("Merging Features");

        glDisable(GL_STENCIL_TEST);
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

    std::vector<match> match_features(const std::vector<feature> & a, const std::vector<feature> & b, const match_settings& settings)
    {
        std::map<float, std::pair<const sift::feature*, const sift::feature*>, std::greater<float>> amatches;
        std::map<float, const sift::feature*, std::greater<float>> vec;

        struct distance_compute
        {
            float operator()(const sift::feature* a, const sift::feature* b) const
            {
                return cosine_similarity(a->descriptor.histrogram.data(), b->descriptor.histrogram.data(), 128);
            }
        } compute;

        for (auto& fta : a)
        {
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
                amatches.emplace(first->first, std::make_pair(&fta, first->second));
            }
        }

        std::vector<match> features;
        features.reserve(amatches.size());
        for (auto const& m : amatches)
        {
            features.emplace_back(match{ *m.second.first, *m.second.second, m.first });
        }
        return features;
    }
}
