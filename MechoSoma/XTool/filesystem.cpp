#include "filesystem.h"

#include <algorithm>
#include <locale>
#include <stdexcept>

std::filesystem::path file::normalize_path(const char* input) {
  namespace fs = std::filesystem;

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

  auto result_path = fs::current_path();
  auto result_path_item = result_path.begin();

  for (const auto& part : input_path) {
    const auto part_name = part.filename().string();
    const auto name = result_path_item->filename().string();
    if (std::equal(part_name.cbegin(), part_name.cend(), name.cbegin(), comparator)) {
      result_path_item++;
      continue;
    }

    bool is_found = false;
    for (const auto& entry : fs::directory_iterator{result_path}) {
      const auto entry_name = entry.path().filename().string();
      if (std::equal(part_name.cbegin(), part_name.cend(), entry_name.cbegin(), comparator)) {
        result_path = entry.path();
        is_found = true;
        break;
      }
    }

    if (!is_found) {
      return input_path;
    }
  }

  return result_path.string();
}
