#pragma once

#include <vector>
#include <array>
#include <functional>

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

    std::vector<feature> detect_features(const image& img, const detection_settings& settings);
    std::vector<match> match_features(const std::vector<feature>& a, const std::vector<feature>& b, const match_settings& settings);
}