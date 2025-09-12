#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <string_view>

#include <aci_parser.h>

#include "common.h"
#include "localization.h"
#include "localization_file.h"

namespace
{

struct SCRDataBlockDeleter final
{
	void operator()(const scrDataBlock *block)
	{
		delete block;
	}
};
using SCRDataBlockPtr = std::unique_ptr<scrDataBlock, SCRDataBlockDeleter>;

}

int handle_localization(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: localization <game data directory> <output file>" << std::endl;
		return -1;
	}

	text::LocalizationFile file(argv[0]);
	file.Save(argv[1]);

	return 0;
}

int handle_scripts(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: scripts <localization file> <game data directory>" << std::endl;
		return -1;
	}

	text::Localization localization;
	localization.Load(argv[0]);

	const auto languages_path = std::filesystem::path(argv[1]) / "lang";
	if (!std::filesystem::exists(languages_path) || !std::filesystem::is_directory(languages_path))
	{
		throw std::runtime_error("lang folder not found");
	}

	getLocalizedKey = [&localization](const std::string_view key) -> const std::string_view {
		return localization.GetLocalizedKey(key);
	};

	scrSetVerbose(0);
	scrSetInputDir("");
	for (const auto &script_file : common::script_files)
	{
		for (const auto &codepage : common::codepages)
		{
			localization.SetLanguage(codepage.language_code);

			SCRDataBlockPtr block(parseScript(script_file.script_file, script_file.set_file));
			if (block == nullptr)
			{
				throw std::runtime_error(std::format("failed to parse script={}", script_file.script_file));
			}

			const auto file_path = languages_path / codepage.language_code / script_file.script_binary_file;
			saveScript(file_path.c_str(), block.get());
		}
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: <command>" << std::endl;
		return -1;
	}

	const std::string_view command(argv[1]);
	if (command == "localization")
	{
		return handle_localization(argc - 2, argv + 2);
	}

	if (command == "scripts")
	{
		return handle_scripts(argc - 2, argv + 2);
	}

	std::cout << std::format("ERROR: unknown command={}", command) << std::endl;
	return -1;
}
