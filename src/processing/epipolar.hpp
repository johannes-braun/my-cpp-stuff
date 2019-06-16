#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace mpp
{
    glm::mat3 ransac_fundamental(std::vector<std::pair<glm::vec2, glm::vec2>> matches);
}