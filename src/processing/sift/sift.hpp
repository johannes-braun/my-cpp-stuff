#pragma once

#include <processing/image.hpp>
#include <opengl/mygl.hpp>
#include <array>
#include <iostream>

namespace mpp::sift
{
    std::vector<image> difference_of_gaussian(const image& img, int octaves, int scales);
}