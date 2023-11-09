#include "filesystem.h"

#include <algorithm>
#include <cstring>
#include <locale>
#include <stdexcept>
#include <unordered_set>

namespace localization {
    std::unordered_set<std::string> localizedFiles = {
          "resource/iscreen/itext.scb",
          "resource/iscreen/iscreen.scb",
          "resource/iscreen/doggy.scb",
          "resource/iscreen/intro/splash.jpg",
          "resource/iscreen/intro_movie/1.jpg",
          "resource/iscreen/intro_movie/2.jpg",
          "resource/iscreen/intro_movie/3.jpg",
          "resource/iscreen/intro_movie/sound/intro_2_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_3_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_3_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_4_3.mp+",
          "resource/iscreen/intro_movie/sound/intro_2_3.mp+",
          "resource/iscreen/intro_movie/sound/intro_1_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_2_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_5_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_4_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_6_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_7_3.mp+",
          "resource/iscreen/intro_movie/sound/intro_5_3.mp+",
          "resource/iscreen/intro_movie/sound/intro_1_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_7_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_4_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_1_3.mp+",
          "resource/iscreen/intro_movie/sound/intro_7_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_5_1.mp+",
          "resource/iscreen/intro_movie/sound/intro_6_2.mp+",
          "resource/iscreen/intro_movie/sound/intro_6_3.mp+",
          "resource/iscreen/intro_movie/sound/intro_7_4.mp+",
          "resource/iscreen/arcane/start/go1.tga",
          "resource/iscreen/arcane/start/go2.tga",
          "resource/iscreen/arcane/start/go.tga",
          "resource/mch_tracks.scb",
          "resource/m3d/s.3ds",
          "resource/m3d/a.3ds",
          "resource/m3d/r.3ds",
          "resource/m3d/t_red.3ds",
          "resource/m3d/r_red.3ds",
          "resource/m3d/s_red.3ds",
          "resource/m3d/a_red.3ds",
          "resource/m3d/t.3ds",
          "resource/m3d.scb",
    };
    std::string langDir = "/lang/ru";

    void setLanguage(const char* lang) {
      langDir = std::string("/lang/") + lang + "/";
    }

    std::string getLocalizedFile(const std::string& input) {
      std::string lower{input};
      std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){
          return std::tolower(c == '\\' ? '/' : c);
      });
      if (localizedFiles.find(lower) != localizedFiles.end()) {
        return langDir + input;
      }
      return input;
    }
}

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
  fs::path input_path{localization::getLocalizedFile(t)};

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
    if (_iterator->is_regular_file() && match(_path.u8string().c_str(), _iterator->path().filename().u8string().c_str())) {
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
