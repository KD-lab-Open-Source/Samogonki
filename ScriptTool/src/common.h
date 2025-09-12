#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <string_view>

namespace common
{

struct ScriptFile final
{
	const char *script_file;
	const char *set_file;
	std::string_view script_binary_file;
};

constexpr std::array<ScriptFile, 4> script_files{
	ScriptFile{
		.script_file = "iScreen/SCRIPTS/ISCREEN.SCR",
		.set_file = "iScreen/SCRIPTS/acs_scr.set",
		.script_binary_file = "RESOURCE/iScreen/iscreen.scb"
	},
	ScriptFile{
		.script_file = "iScreen/SCRIPTS/doggy.scr",
		.set_file = "iScreen/SCRIPTS/acs_scr.set",
		.script_binary_file = "RESOURCE/iScreen/doggy.scb"
	},
	ScriptFile{
		.script_file = "iScreen/SCRIPTS/iText.scr",
		.set_file = "iScreen/SCRIPTS/acs_scr.set",
		.script_binary_file = "RESOURCE/iScreen/iText.scb"
	},
	ScriptFile{
		.script_file = "RUNTIME/Scripts/mch_tracks.scr",
		.set_file = "RUNTIME/Scripts/mch_script.set",
		.script_binary_file = "RESOURCE/mch_tracks.scb"
	}
};

struct Codepage final
{
	const char *language_code;
	const char *codepage;
};

constexpr std::array<Codepage, 6> codepages{
	Codepage{"cz", "cp1250"},
	Codepage{"en", "cp1250"},
	Codepage{"fr", "cp1250"},
	Codepage{"it", "cp1250"},
	Codepage{"lt", "cp1250"},
	Codepage{"ru", "cp1251"}
};

std::string load_file(const std::filesystem::path &file_path);

}
