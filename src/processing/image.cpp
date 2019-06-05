#include <processing/image.hpp>
#include <nothings/stb_image.h>

namespace mpp
{
    image::image(image&& img) noexcept
        : _width(img._width), _height(img._height), _components(img._components), _data(std::move(img._data))
    {
        img._width = 0;
        img._height = 0;
        img._components = 0;
    }

    image& image::operator=(image&& img) noexcept
    {
        _width = img._width;
        _height = img._height;
        _components = img._components;
        _data = std::move(img._data);
        img._width = 0;
        img._height = 0;
        img._components = 0;
        return *this;
    }

    void image::load_empty(std::int32_t width, std::int32_t height, std::int32_t components)
    {
        _data.resize(size_t(_width = width) * (_height = height) * (_components = components));
    }

    void image::load_stream(std::istream& stream, std::optional<std::int32_t> desired_components)
    {
        stbi_io_callbacks callbacks;
        callbacks.read = [](void* user, char* data, int size) -> int {
            std::istream& stream = *static_cast<std::istream*>(user);
            return static_cast<int>(stream.read(data, size).gcount());
        };
        callbacks.skip = [](void* user, int n) -> void {
            std::istream& stream = *static_cast<std::istream*>(user);
            stream.ignore(n);
        };
        callbacks.eof = [](void* user) -> int {
            std::istream& stream = *static_cast<std::istream*>(user);
            return stream.eof() ? 1 : 0;
        };
        auto* data = stbi_load_from_callbacks(&callbacks, &stream, &_width, &_height, &_components, desired_components ? *desired_components : 0);
        _components = desired_components ? *desired_components : _components;
        _data.resize(size_t(_width) * _height * _components);
        _data.assign(data, data + _data.size());
    }
    glm::ivec2 image::dimensions() const noexcept
    {
        return glm::ivec2(_width, _height);
    }
    size_t image::size() const noexcept
    {
        return _data.size();
    }
    std::int32_t image::components() const noexcept
    {
        return _components;
    }
    char* image::data() noexcept
    {
        return _data.data();
    }
    const char* image::data() const noexcept
    {
        return _data.data();
    }
    void image::store(std::int32_t x, std::int32_t y, glm::vec4 value)
    {
        char* d = &_data[size_t(y) * _width * _components + size_t(x) * _components];
        for (int i = 0; i < _components; ++i)
        {
            d[i] = char(value[i] * 255.f);
        }
    }
    glm::vec4 image::load(std::int32_t x, std::int32_t y) const
    {
        const char* d = &_data[size_t(y) * _width * _components + size_t(x) * _components];
        glm::vec4 val(0, 0, 0, 1);
        for (int i = 0; i < _components; ++i)
        {
            val[i] = float(d[i]) / 255.f;
        }
        return val;
    }
}