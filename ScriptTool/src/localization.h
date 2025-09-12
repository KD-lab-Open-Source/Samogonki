#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <string_view>

namespace text
{

namespace fs = std::filesystem;

class Localization final
{
public:
	void Load(const fs::path &file_path);
	void SetLanguage(const std::string_view language);
	const std::string_view GetLocalizedKey(const std::string_view key) const;

private:
	using Text = std::map<std::string, std::string>;
	std::map<std::string, Text> _text;
	std::string_view _language;
};

}
