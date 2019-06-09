#pragma once

#include <vector>

namespace mpp {
    class image;
}

namespace mpp::sift
{
    struct feature
    {
        float x;
        float y;
        float sigma;
        float scale;
        float orientation;
    };

    std::vector<feature> sift(const image& img, size_t octaves, size_t feature_scales);
}