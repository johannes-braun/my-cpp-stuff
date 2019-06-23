#pragma once

#include <optional>
#include <filesystem>
#include <string>
#include <vector>

namespace mpp
{
    std::optional<std::filesystem::path> open_file(const std::string& title, const std::filesystem::path& default_path = "./", const std::vector<const char*>& filter_patterns = {}, const char* description = nullptr);
    std::vector<std::filesystem::path> open_files(const std::string& title, const std::filesystem::path& default_path = "./", const std::vector<const char*>& filter_patterns = {}, const char* description = nullptr);
}