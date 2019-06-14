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
        enum class file_format
        {
            jpg, png, bmp
        };

        image() = default;
        image(image&& img) noexcept;
        image(const image& img) = default;
        image& operator=(image&& img) noexcept;
        image& operator=(const image& img) = default;

        void load_empty(std::int32_t width, std::int32_t height, std::int32_t components);
        void load_stream(std::istream& stream, std::optional<std::int32_t> desired_components = std::nullopt);
        void load_stream(std::istream&& stream, std::optional<std::int32_t> desired_components = std::nullopt);

        void save_stream(std::ostream& stream, file_format fmt = file_format::png);        
        void save_stream(std::ostream&& stream, file_format fmt = file_format::png);

        image& resize(int width, int height);

        glm::ivec2 dimensions() const noexcept;
        size_t size() const noexcept;
        std::int32_t components() const noexcept;
        char* data() noexcept;
        const char* data() const noexcept;
        void write(std::int32_t x, std::int32_t y, glm::vec4 value);
        glm::vec4 read(std::int32_t x, std::int32_t y) const;

    private:
        std::int32_t _width = 0;
        std::int32_t _height = 0;
        std::int32_t _components = 0;
        std::vector<char> _data;
    };
}