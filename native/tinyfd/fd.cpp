#include <tinyfd/fd.hpp>
#include <tinyfd/tinyfiledialogs.h>
#include <sstream>

namespace mpp
{
    std::optional<std::filesystem::path> open_file(const std::string& title, const std::filesystem::path& default_path, const std::vector<const char*>& filter_patterns, const char* description)
    {
        const char* path = tinyfd_openFileDialog(title.c_str(), default_path.string().c_str(), int(filter_patterns.size()), filter_patterns.data(), description, false);
        if (path)
            return path;
        return std::nullopt;
    }

    std::vector<std::filesystem::path> open_files(const std::string& title, const std::filesystem::path& default_path, const std::vector<const char*>& filter_patterns, const char* description)
    {
        const char* paths = tinyfd_openFileDialog(title.c_str(), default_path.string().c_str(), int(filter_patterns.size()), filter_patterns.data(), description, true);

        if (!paths)
            return {};

        std::vector<std::filesystem::path> ret;
        std::istringstream ss(paths);
        std::string token;

        while (std::getline(ss, token, '|')) {
            ret.emplace_back(token);
        }
        return ret;
    }
}
