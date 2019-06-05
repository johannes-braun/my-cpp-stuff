#include <processing/sift/sift.hpp>

namespace mpp::sift
{
    std::vector<image> difference_of_gaussian(const image& img, int octaves, int scales)
    {
        std::vector<std::uint32_t> textures(scales); // one texture per scale with N octaves (mipmaps)
        std::uint32_t temp_texture;
        std::array<std::uint32_t, 2> framebuffers;
        glGenTextures(scales, textures.data());
        glGenTextures(1, &temp_texture);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.dimensions().x, img.dimensions().y, 0, fmt, GL_UNSIGNED_BYTE, i == 0 ? img.data() : nullptr);
        }
        glBindTexture(GL_TEXTURE_2D, temp_texture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.dimensions().x, img.dimensions().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGenFramebuffers(2, framebuffers.data());

        std::uint32_t gauss_fs = glCreateShader(GL_FRAGMENT_SHADER);
        constexpr auto gauss_fs_src = R"(#version 330 core
layout(location = 0) in vec2 in_uv;
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
    for(int i=1; i<int(6*u_sigma); ++i)
    {
        off[u_dir] = i;
        gval = gaussian(u_sigma, float(i));
        color += gval * (texelFetch(in_texture, uv + off, u_mip) 
            + texelFetch(in_texture, uv - off, u_mip));
    }
    out_color = vec4(1, 1, 1, 1);//color;
}
)";
        glShaderSource(gauss_fs, 1, &gauss_fs_src, nullptr);
        glCompileShader(gauss_fs);

        std::uint32_t screen_vs = glCreateShader(GL_VERTEX_SHADER);
        constexpr auto screen_vs_src = R"(#version 330 core
layout(location = 0) out vec2 out_uv;
void main()
{
    gl_Position = vec4(mix(-1, 3, gl_VertexID & 0x1), mix(-1, 3, (gl_VertexID >> 1) & 0x1), 0.f, 1.f);
    out_uv = ((gl_Position.xy+1)*0.5f);
}
)";
        glShaderSource(screen_vs, 1, &screen_vs_src, nullptr);
        glCompileShader(screen_vs);

        const auto gauss_program = glCreateProgram();
        glAttachShader(gauss_program, screen_vs);
        glAttachShader(gauss_program, gauss_fs);
        glLinkProgram(gauss_program);

        int log_len;
        glGetProgramiv(gauss_program, GL_INFO_LOG_LENGTH, &log_len);
        std::string info_log(log_len, '\0');
        glGetProgramInfoLog(gauss_program, log_len, &log_len, info_log.data());
        std::cout << info_log << '\n';

        glDetachShader(gauss_program, gauss_fs);
        glDetachShader(gauss_program, screen_vs);
        glDeleteShader(gauss_fs);
        glDeleteShader(screen_vs);

        int u_mip_loc = glGetUniformLocation(gauss_program, "u_mip");
        int u_dir_loc = glGetUniformLocation(gauss_program, "u_dir");
        int u_sigma_loc = glGetUniformLocation(gauss_program, "u_sigma");
        int in_texture_loc = glGetUniformLocation(gauss_program, "in_texture");

        glActiveTexture(GL_TEXTURE0);
        glUseProgram(gauss_program);
        glUniform1i(in_texture_loc, 0);

        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Todo: Fix textures not being written.
        // Apply gaussian with sigma scale*diff_sigma.
        for (int scale = 1; scale < scales; ++scale)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
            // Write from textures[0] to temp_texture
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, temp_texture, 0);
            glUniform1i(u_mip_loc, 0);
            glUniform1i(u_dir_loc, 0);
            glUniform1f(u_sigma_loc, scale * 0.3f);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Write from temp_texture to textures[scale]
            glBindTexture(GL_TEXTURE_2D, temp_texture);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[scale], 0);
            glUniform1i(u_dir_loc, 1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindTexture(GL_TEXTURE_2D, textures[scale]);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        // TODO: compute difference of gaussian.
        std::vector<image> output(size_t(octaves) * scales);

        int base_width = img.dimensions().x;
        int base_height = img.dimensions().y;
        for (int scale = 0; scale < scales; ++scale)
        {
            for (int octave = 0; octave < octaves; ++octave)
            {
                const int out_idx = octave * scales + scale;
                output[out_idx].load_empty(base_width >> octave, base_height >> octave, img.components());
                glBindTexture(GL_TEXTURE_2D, textures[scale]);
                glGetTexImage(GL_TEXTURE_2D, octave, fmt, GL_UNSIGNED_BYTE, output[out_idx].data());
            }
        }

        glDeleteProgram(gauss_program);
        glDeleteFramebuffers(2, framebuffers.data());
        glDeleteTextures(scales, textures.data());

        return output;
    }
}