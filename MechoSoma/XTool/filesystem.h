#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace localization
{
    void setLanguage(const char* lang);
    std::string getLocalizedFile(const std::string& input);
}

namespace file
{

std::string normalize_path(const char* input);

class FileFinder
{
public:
    std::optional<std::filesystem::path> find_first(const std::string &path);
    std::optional<std::filesystem::path> find_next();

private:
    static bool match(char const *needle, char const *haystack);

private:
    std::filesystem::path _path;
    std::filesystem::directory_iterator _iterator;
};

}
