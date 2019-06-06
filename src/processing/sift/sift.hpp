#pragma once

#include <vector>

namespace mpp {
    class image;
}

namespace mpp::sift
{
    std::vector<image> sift(const image& img, size_t octaves, size_t feature_scales);
}