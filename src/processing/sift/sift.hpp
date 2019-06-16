#pragma once

#include <vector>
#include <array>
#include <functional>
#include <glm/glm.hpp>

namespace mpp {
    class image;
}

namespace mpp::sift
{
    struct detection_settings
    {
        size_t octaves = 3;
        size_t feature_scales = 3;
        int orientation_slices = 36;
        float orientation_magnitude_threshold = 0.0002f;
    };

    struct match_settings
    {
        float relation_threshold = 0.8f;
        float similarity_threshold = 0.9f; // > 90% matches
        int max_match_count = std::numeric_limits<int>::max();
    };

    enum class dst_system
    {
        pixel_coordinates, // results in range {[0, img.w), [0, img.h)}
        image_coordinates, // results in range {[0, 1), [0, 1)}
        normalized_coordinates // results in range {(-1, 1), (-1, 1)}
    };

    struct feature
    {
        float x;
        float y;
        float sigma;
        float scale;
        float orientation; // angle in radians
        int octave;
        struct descriptor_t
        {
            std::array<float, 128> histrogram{ 0 };
        } descriptor;
    };

    struct match
    {
        feature a;
        feature b;
        float similarity;
    };
    struct sift_cache;
    std::shared_ptr<sift_cache> create_cache(size_t num_octaves, size_t num_feature_scales);

    std::vector<feature> detect_features(const image& img, const detection_settings& settings, dst_system system = dst_system::pixel_coordinates);
    std::vector<feature> detect_features(sift_cache& cache, const image& img, const detection_settings& settings, dst_system system = dst_system::pixel_coordinates);
    std::vector<match> match_features(const std::vector<feature>& a, const std::vector<feature>& b, const match_settings& settings);
    std::vector<std::pair<glm::vec2, glm::vec2>> corresponding_points(const std::vector<match>& matches);
}