#include <processing/sift/sift.hpp>

namespace mpp::sift
{
    std::vector<image> difference_of_gaussian(const image& img, int octaves, int scales)
    {
        assert(scales > 3);
        const auto time_begin = std::chrono::steady_clock::now();

        // use one more temporary scale for the intermediate gaussian results.
        ++scales;
        std::vector<std::uint32_t> textures(scales); // one texture per scale with N octaves (mipmaps)
        std::vector<std::uint32_t> features((size_t(scales)-3) * octaves); // middle (n-2) textures, without the increment
        std::array<std::uint32_t, 2> temp_textures;
        std::vector<std::uint32_t> framebuffers(octaves);
        glGenTextures(scales, textures.data());
        glGenTextures(int(features.size()), features.data());
        glGenTextures(int(temp_textures.size()), temp_textures.data());
        const auto fmt = [&] {
            switch (img.components())
            {
            case 1: return GL_RED;
            case 2: return GL_RG;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
            default: return GL_INVALID_VALUE;
            }
        }();
        for (int i = 0; i < scales; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, img.dimensions().x, img.dimensions().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }
        for (int i = 0; i < features.size(); ++i)
        {
            glBindTexture(GL_TEXTURE_2D, features[i]);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, img.dimensions().x, img.dimensions().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, temp_textures[0]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, img.dimensions().x, img.dimensions().y, 0, fmt, GL_UNSIGNED_BYTE, img.data());
        glBindTexture(GL_TEXTURE_2D, temp_textures[1]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, img.dimensions().x, img.dimensions().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGenFramebuffers(int(framebuffers.size()), framebuffers.data());
        std::uint32_t empty_vao;
        glGenVertexArrays(1, &empty_vao);

        std::uint32_t gauss_fs = glCreateShader(GL_FRAGMENT_SHADER);
        constexpr auto gauss_fs_src = R"(#version 330 core
in vec2 vs_uv;
uniform int u_mip;
uniform int u_dir;
uniform float u_sigma;
uniform sampler2D in_texture;
layout(location = 0) out vec4 out_color;

float gaussian(float sigma, float diff)
{
    const float sqrt_2_pi = 2.50662827463f;
    float inner = diff/sigma;
    float nom = exp(-(inner * inner / 2));
    return nom / (sigma * sqrt_2_pi);
}

void main()
{
    ivec2 uv = ivec2(gl_FragCoord.xy);
    float gval = gaussian(u_sigma, 0.f);
    vec4 color = gval * texelFetch(in_texture, uv, u_mip);
    ivec2 off = ivec2(0, 0);
    ivec2 tsize = ivec2(textureSize(in_texture, u_mip).xy);
    for(int i=1; i<int(6*u_sigma); ++i)
    {
        off[u_dir] = i;
        ivec2 pos_uv = ivec2(mod(uv + off, tsize));
        ivec2 neg_uv = ivec2(mod(uv + tsize - off, tsize));
        gval = gaussian(u_sigma, float(i));
        color += gval * (texelFetch(in_texture, pos_uv, u_mip) 
            + texelFetch(in_texture, neg_uv, u_mip));
    }
    out_color = color;
}
)";
        glShaderSource(gauss_fs, 1, &gauss_fs_src, nullptr);
        glCompileShader(gauss_fs);

        std::uint32_t screen_vs = glCreateShader(GL_VERTEX_SHADER);
        constexpr auto screen_vs_src = R"(#version 330 core
out vec2 vs_uv;
void main()
{
    gl_Position = vec4(mix(-1.f, 3.f, float(gl_VertexID & 0x1)), mix(-1.f, 3.f, float((gl_VertexID >> 1) & 0x1)), 0.f, 1.f);
    vs_uv = ((gl_Position.xy+1)*0.5f);
}
)";
        glShaderSource(screen_vs, 1, &screen_vs_src, nullptr);
        glCompileShader(screen_vs);

        const auto gauss_program = glCreateProgram();
        glAttachShader(gauss_program, screen_vs);
        glAttachShader(gauss_program, gauss_fs);
        glLinkProgram(gauss_program);

        const auto check_program = [](auto program) {
            int log_len;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
            std::string info_log(log_len, '\0');
            glGetProgramInfoLog(program, log_len, &log_len, info_log.data());
            std::cout << info_log << '\n';
        };
        check_program(gauss_program);
        glDeleteShader(gauss_fs);

        int u_mip_loc = glGetUniformLocation(gauss_program, "u_mip");
        int u_dir_loc = glGetUniformLocation(gauss_program, "u_dir");
        int u_sigma_loc = glGetUniformLocation(gauss_program, "u_sigma");
        int in_texture_loc = glGetUniformLocation(gauss_program, "in_texture");

        std::uint32_t diff_fs = glCreateShader(GL_FRAGMENT_SHADER);
        constexpr auto diff_fs_src = R"(#version 330 core
in vec2 vs_uv;
uniform sampler2D u_current_tex;
uniform sampler2D u_previous_tex;
layout(location = 0) out vec4 color;
void main()
{
    ivec2 px = ivec2(gl_FragCoord.xy);
    color = abs(texelFetch(u_current_tex, px, 0) - texelFetch(u_previous_tex, px, 0));
}
)";
        glShaderSource(diff_fs, 1, &diff_fs_src, nullptr);
        glCompileShader(diff_fs);

        const auto diff_program = glCreateProgram();
        glAttachShader(diff_program, screen_vs);
        glAttachShader(diff_program, diff_fs);
        glLinkProgram(diff_program);
        check_program(diff_program);
        int u_current_tex_loc = glGetUniformLocation(diff_program, "u_current_tex");
        int u_previous_tex_loc = glGetUniformLocation(diff_program, "u_previous_tex");

        std::uint32_t max_fs = glCreateShader(GL_FRAGMENT_SHADER);
        constexpr auto max_fs_src = R"(#version 330 core
in vec2 vs_uv;
uniform sampler2D u_previous_tex;
uniform sampler2D u_current_tex;
uniform sampler2D u_next_tex;
uniform int u_mip;
uniform int u_neighbors; // 0 both, 1 only next, -1 only prev
layout(location = 0) out vec4 color;
void main()
{
    ivec2 px = ivec2(gl_FragCoord.xy);
    // look at neighbors
    //float max_val = -1.f/0.f;
    //float min_val = 1.f/0.f;
    ivec2 tsize = ivec2(textureSize(u_current_tex, u_mip).xy);
    float val_curr = texelFetch(u_current_tex, px, u_mip).r;
    float cmax_val = -1.f/0.f;
    float cmin_val = 1.f/0.f;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            if(x == 0 && y == 0)
                continue;
            ivec2 pos_uv = ivec2(clamp(px + ivec2(x, y), ivec2(0, 0), tsize - 1));
            float vprev = u_neighbors == 1 ? -(1.f/0.f) : texelFetch(u_previous_tex, pos_uv, u_mip).r;
            float vcurr = texelFetch(u_current_tex, pos_uv, u_mip).r;
            float vnext = u_neighbors == -1 ? -(1.f/0.f) : texelFetch(u_next_tex, pos_uv, u_mip).r;
            
            float maxval = max(vprev, max(vcurr, vnext));
            float minval = min(vprev, min(vcurr, vnext));
            if(maxval > cmax_val)
            {
                cmax_val = maxval;
            }
            if(minval < cmin_val)
            {
                cmin_val = minval;
            }
        }
    }
    if((cmax_val < val_curr || cmin_val > val_curr) && max(abs(cmax_val - val_curr), abs(cmin_val - val_curr)) > 0.01f)
    {
        color = vec4(1, 1, 1, 1);
    }
    else
        color = vec4(0, 0, 0, 1);
}
)";
        glShaderSource(max_fs, 1, &max_fs_src, nullptr);
        glCompileShader(max_fs); 
        const auto max_program = glCreateProgram();
        glAttachShader(max_program, screen_vs);
        glAttachShader(max_program, max_fs);
        glLinkProgram(max_program);
        check_program(max_program);
        int u_previous_tex_max_loc = glGetUniformLocation(max_program, "u_previous_tex");
        int u_current_tex_max_loc = glGetUniformLocation(max_program, "u_current_tex");
        int u_next_tex_max_loc = glGetUniformLocation(max_program, "u_next_tex");
        int u_neighbors_max_loc = glGetUniformLocation(max_program, "u_neighbors");
        int u_mip_max_loc = glGetUniformLocation(max_program, "u_mip");

        glDeleteShader(screen_vs);

        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glGenerateMipmap(GL_TEXTURE_2D);
        glm::ivec4 viewport;
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        
        int base_width = img.dimensions().x;
        int base_height = img.dimensions().y;
        glViewport(0, 0, base_width, base_height);

        glActiveTexture(GL_TEXTURE0);
        glUseProgram(gauss_program);
        glUniform1i(in_texture_loc, 0);

        // Apply gaussian with sigma scale*diff_sigma.
        for (int scale = 0; scale < scales; ++scale)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
            // Write from temp_textures[0] (original) to temp_textures[1]
            glBindTexture(GL_TEXTURE_2D, temp_textures[0]);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, temp_textures[1], 0);
            glUniform1i(u_mip_loc, 0);
            glUniform1i(u_dir_loc, 0);
            glUniform1f(u_sigma_loc, pow(sqrt(2), scale+1) * 2.f);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Write from temp_textures[1] to textures[scale]
            glBindTexture(GL_TEXTURE_2D, temp_textures[1]);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[scale], 0);
            glUniform1i(u_dir_loc, 1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glUseProgram(diff_program);
        glUniform1i(u_current_tex_loc, 0);
        glUniform1i(u_previous_tex_loc, 1);

        // Now compute difference of textures[scale] to previous scale from textures[scale-1]...
        for (int scale = scales-1; scale >= 1; --scale)
        {
            // Write it to textures[scale]
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[scale], 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[scale]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[std::int64_t(scale) - 1]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        for (int i = 0; i < scales; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glUseProgram(max_program);
        glUniform1i(u_previous_tex_max_loc, 0);
        glUniform1i(u_current_tex_max_loc, 1);
        glUniform1i(u_next_tex_max_loc, 2);
        for (int o = 0; o < octaves; ++o)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[o]);
            glViewport(0, 0, base_width << o, base_height << o);
            for (size_t feature_layer = 1; feature_layer <= (size_t(scales) - 3); ++feature_layer)
            {
                const auto layer_id = feature_layer - 1 + (o * (size_t(scales) - 3));
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, features[layer_id], o);
                const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[std::int64_t(feature_layer)]);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, textures[std::int64_t(feature_layer) + 1]);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, textures[std::int64_t(feature_layer) + 2]);
                glUniform1i(u_neighbors_max_loc, 0);
                glUniform1i(u_mip_max_loc, o);

                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
        }

        std::vector<image> output(features.size());

        struct feature
        {
            float x;
            float y;
            float sigma;
        };
        std::vector<std::vector<feature>> feature_points(octaves);
        
        for (int scale = 0; scale < scales-3; ++scale)
        {
            for (int octave = 0; octave < octaves; ++octave)
            {
                const int out_idx = octave * (scales-3) + scale;
                output[out_idx].load_empty(base_width >> octave, base_height >> octave, img.components());
                glBindTexture(GL_TEXTURE_2D, features[out_idx]);
                glGetTexImage(GL_TEXTURE_2D, octave, fmt, GL_UNSIGNED_BYTE, output[out_idx].data());

                for (int y = 0; y < base_height >> octave; ++y)
                {
                    for (int x = 0; x < base_width >> octave; ++x)
                    {
                        if (output[out_idx].data()[x + y * output[out_idx].dimensions().x] != 0)
                        {
                            feature_points[octave].push_back(feature{float(x), float(y), float(scale)});
                        }
                    }
                }
            }
        }

        //TODO: approximate quadratic taylor and make features more accurate

        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

        glDeleteProgram(gauss_program);
        glDeleteFramebuffers(int(framebuffers.size()), framebuffers.data());
        glDeleteTextures(scales, textures.data());
        glDeleteTextures(int(temp_textures.size()), temp_textures.data());
        glDeleteTextures(int(features.size()), features.data());
        glDeleteVertexArrays(1, &empty_vao);

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_begin).count() << "ms\n";

        return output;
    }
}