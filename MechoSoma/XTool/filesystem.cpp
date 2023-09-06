#include "filesystem.h"

#include <algorithm>
#include <cstring>
#include <locale>
#include <stdexcept>

namespace fs = std::filesystem;

std::string file::normalize_path(const char* input) {
  const auto current_locale = std::locale();
  auto comparator = [&current_locale](char left, char right) {
    return std::tolower(left, current_locale) == std::tolower(right, current_locale);
  };

  std::string t{input};
  for (auto& c : t) {
    if (c == '\\') {
      c = '/';
    }
  }
  fs::path input_path{t};

  auto current_path = fs::current_path();
  auto current_path_part = current_path.begin();
  auto result_path = current_path;

  for (const auto& part : input_path) {
    const auto part_name = part.filename().string();
    if (current_path_part != current_path.end()) {
      const auto name = current_path_part->filename().string();
      if (std::equal(part_name.cbegin(), part_name.cend(), name.cbegin(), name.cend(), comparator)) {
        current_path_part++;
        continue;
      }
    }

    bool is_found = false;
    for (const auto& entry : fs::directory_iterator{result_path}) {
      const auto entry_name = entry.path().filename().string();
      if (std::equal(part_name.cbegin(), part_name.cend(), entry_name.cbegin(), entry_name.cend(), comparator)) {
        result_path = entry.path();
        is_found = true;
        break;
      }
    }

    if (!is_found) {
      return input_path.string();
    }
  }

  return result_path.string();
}

using namespace file;

std::optional<fs::path> FileFinder::find_first(const std::string &path) {
  _path = path;
  _iterator = fs::directory_iterator(fs::current_path());
  return find_next();
}

std::optional<fs::path> FileFinder::find_next() {
  while (_iterator != std::filesystem::end(_iterator)) {
    if (_iterator->is_regular_file() && match(_path.c_str(), _iterator->path().filename().string().c_str())) {
      const auto p = _iterator->path();
      _iterator++;
      return p.filename();
    }
    _iterator++;
  }
  return std::nullopt;
}

// https://stackoverflow.com/questions/3300419/file-name-matching-with-wildcard
bool FileFinder::match(char const *needle, char const *haystack) {
  for (; *needle != '\0'; ++needle) {
    switch (*needle) {
    case '?':
      if (*haystack == '\0') {
        return false;
      }
      ++haystack;
      break;

    case '*': {
      if (needle[1] == '\0') {
        return true;
      }
      size_t max = std::strlen(haystack);
      for (size_t i = 0; i < max; i++) {
        if (match(needle + 1, haystack + i)) {
          return true;
        }
      }
      return false;
    }

    default:
      if (*haystack != *needle) {
        return false;
      }
      ++haystack;
    }
  }
  return *haystack == '\0';
}
