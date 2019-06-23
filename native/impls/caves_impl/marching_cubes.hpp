#pragma once

#include <cstdint>
#include <array>
#include <glm/glm.hpp>
#include <impls/caves_impl/marching_cubes_tables.inl>

namespace mpp
{
    constexpr std::array<std::int32_t, 256> const & case_faces = detail::case_faces;
    constexpr std::array<detail::edge_set_t, 256> const& edge_connections = detail::edge_connections;
}