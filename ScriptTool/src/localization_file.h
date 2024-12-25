#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "common.h"

class scrDataBlock;

namespace text
{

namespace fs = std::filesystem;

class LocalizationFile final
{
public:
	explicit LocalizationFile(const fs::path &game_data);
	void Save(const fs::path &output_file);

private:
	void LoadLocalizationKeys(const fs::path &strings_file);
	bool IsLocalizedKey(const std::string_view key) const;
	void ProcessScript(const common::ScriptFile &script_file);
	void ProcessScript(
		scrDataBlock *source_block,
		const fs::path &language_path,
		const common::ScriptFile &script_file,
		const common::Codepage &codepage
	);

	const fs::path _game_data;
	using Text = std::map<std::string, std::string>;
	std::map<std::string, Text> _text;	
};

}
