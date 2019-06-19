#pragma once

#include <vector>
#include <cstdint>

namespace mpp::sift::detail
{
    struct sift_state
    {
        using uniform_t = std::int32_t;

        sift_state(size_t num_octaves, size_t num_feature_scales);
        void resize(int width, int height);
        ~sift_state();

        sift_state(const sift_state&) = delete;
        sift_state(sift_state&&) = delete;
        sift_state& operator=(const sift_state&) = delete;
        sift_state& operator=(sift_state&&) = delete;

        size_t num_octaves;
        size_t num_feature_scales;
        std::vector<std::uint32_t> temporary_textures;
        std::vector<std::uint32_t> gaussian_textures;
        std::vector<std::uint32_t> difference_of_gaussian_textures;
        std::vector<std::uint32_t> feature_textures;
        std::vector<std::uint32_t> orientation_textures;
        std::vector<std::uint32_t> feature_stencil_buffers;
        std::vector<std::uint32_t> framebuffers;
        std::uint32_t transform_feedback_buffer;
        std::uint32_t empty_vao;

        struct {
            std::uint32_t program;
            uniform_t  u_mip_location;
            uniform_t  u_dir_location;
            uniform_t  u_sigma_location;
            uniform_t  in_texture_location;                    
        } gauss_blur;

        struct {
            std::uint32_t program;
            uniform_t  u_current_tex_location;
            uniform_t  u_previous_tex_location;
        } difference;

        struct {
            std::uint32_t program;
            uniform_t  u_previous_tex_location;
            uniform_t  u_current_tex_location;
            uniform_t  u_next_tex_location;
            uniform_t  u_neighbors_location;
            uniform_t  u_mip_location;
            uniform_t  u_scale_location;
        } maximize;

        struct {
            std::uint32_t program;
            uniform_t u_previous_tex_location;
            uniform_t u_current_tex_location;
            uniform_t u_next_tex_location;
            uniform_t u_feature_tex_location;
            uniform_t u_scale_location;
            uniform_t u_mip_location;
            uniform_t u_border_location;
        } filter;

        struct {
            std::uint32_t program;
            uniform_t u_texture_location;
            uniform_t u_mip_location;
        } transform_feedback_reduce;
    };
}