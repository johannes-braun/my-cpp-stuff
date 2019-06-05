#pragma once

#include <vector>
#include <filesystem>
#include <optional>
#include <glm/glm.hpp>

namespace mpp
{
    class image
    {
    public:
        image() = default;
        image(image&& img) noexcept;
        image(const image& img) = default;
        image& operator=(image&& img) noexcept;
        image& operator=(const image& img) = default;

        void load_empty(std::int32_t width, std::int32_t height, std::int32_t components);
        void load_stream(std::istream& stream, std::optional<std::int32_t> desired_components = std::nullopt);

        glm::ivec2 dimensions() const noexcept;
        size_t size() const noexcept;
        std::int32_t components() const noexcept;
        char* data() noexcept;
        const char* data() const noexcept;
        void store(std::int32_t x, std::int32_t y, glm::vec4 value);
        glm::vec4 load(std::int32_t x, std::int32_t y) const;

    private:
        std::int32_t _width;
        std::int32_t _height;
        std::int32_t _components;
        std::vector<char> _data;
    };
}