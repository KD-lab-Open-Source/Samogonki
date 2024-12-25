#include "localization_file.h"

#include <array>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>

#include <SCRIPTS/SCR_DEFS.H>
#include <aci_parser.h>

#include "text_converter.h"

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

using namespace text;

LocalizationFile::LocalizationFile(const fs::path &game_data) : _game_data(game_data) {}

void LocalizationFile::Save(const fs::path &output_file)
{
	const fs::path strings_file("iScreen/SCRIPTS/ACI_STR.INC");
	if (!fs::exists(strings_file))
	{
		throw std::runtime_error(std::format("{} file not found", strings_file.string()));
	}
	LoadLocalizationKeys(strings_file);

	getLocalizedKey = [this](const std::string_view key) -> const std::string_view {
		return IsLocalizedKey(key) ? key : "";
	};

	scrSetVerbose(0);
	scrSetInputDir("");

	for (const auto &file : common::script_files)
	{
		ProcessScript(file);
	}

	std::ofstream file(output_file, std::ios::out | std::ios::binary);
	if (!file)
	{
		throw std::runtime_error("open file");
	}

	for (const auto &e : _text)
	{
		if (e.second.size() != common::codepages.size())
		{
			std::cout << std::format("key={} has empty text", e.first) << std::endl;
			continue;
		}

		file << std::format("$$$ID\r\n{}\r\n", e.first);
		for (const auto &t : e.second)
		{
			file << std::format("$$$LANGUAGE\r\n{}\r\n$$$TEXT\r\n{}\r\n", t.first, t.second);
		}
	}
}

void LocalizationFile::LoadLocalizationKeys(const fs::path &strings_file)
{
	const auto buffer = common::load_file(strings_file);
	std::regex expression("(#define|#text)\\s+(\\S+)", std::regex_constants::icase);
	std::smatch m;
	auto start = buffer.cbegin();
	while (std::regex_search(start, buffer.cend(), m, expression))
	{
		const auto key = m[2].str();
		if (_text.find(key) != _text.end())
		{
			throw std::runtime_error(std::format("localization key={} duplicate", key));
		}
		_text.emplace(key, Text());
		start = m.suffix().first;
	}
}

bool LocalizationFile::IsLocalizedKey(const std::string_view key) const
{
	return std::find_if(_text.begin(), _text.end(), [&key](const auto &e) { return e.first == key; }) != _text.end();
}

void LocalizationFile::ProcessScript(const common::ScriptFile &script_file)
{
	std::cout << std::format("processing script={}", script_file.script_file) << std::endl;

	SCRDataBlockPtr source_block(parseScript(script_file.script_file, script_file.set_file));
	if (source_block == nullptr)
	{
		throw std::runtime_error(std::format("failed to parse script={}", script_file.script_file));
	}

	const auto languages_path = _game_data / "lang";
	if (!std::filesystem::exists(languages_path) || !std::filesystem::is_directory(languages_path))
	{
		throw std::runtime_error(std::format("lang folder={} not found", languages_path.string()));
	}

	for (const auto &entry : std::filesystem::directory_iterator{languages_path})
	{
		if (!entry.is_directory())
		{
			continue;
		}

		const auto &name = entry.path().filename();
		const auto p = std::find_if(
			common::codepages.cbegin(),
			common::codepages.cend(),
			[&name](const auto &e) -> bool { return e.language_code == name; }
		);
		if (p == common::codepages.end())
		{
			throw std::runtime_error(std::format("language={} not found", name.string()));
		}

		fs::path special_case_path(script_file.script_file);
		special_case_path.replace_filename(
			std::format("{}_{}", p->language_code, special_case_path.filename().string())
		);
		if (fs::exists(special_case_path))
		{
			SCRDataBlockPtr result(parseScript(special_case_path.c_str(), script_file.set_file));
			if (result == nullptr)
			{
				throw std::runtime_error("failed to parse script");
			}
			ProcessScript(result.get(), entry.path(), script_file, *p);
		}
		else
		{
			ProcessScript(source_block.get(), entry.path(), script_file, *p);
		}
	}
}

void LocalizationFile::ProcessScript(
	scrDataBlock *source_block,
	const fs::path &language_path,
	const common::ScriptFile &script_file,
	const common::Codepage &codepage
)
{
	std::cout << std::format("processing script for language={}", codepage.language_code) << std::endl;

	const auto file = language_path / script_file.script_binary_file;
	auto target_block = loadScript(file.c_str());
	if (target_block == nullptr)
	{
		throw std::runtime_error(std::format("failed to parse script", file.string()));
	}

	text::TextConverter text_converter(codepage.codepage, "utf-8");
	std::function<void (scrDataBlock *, scrDataBlock *)> visitor;
	visitor = [&](scrDataBlock *source_block, scrDataBlock *target_block) {
		if (source_block->ID != target_block->ID)
		{
			throw std::runtime_error(std::format("source block ID={} not equal to target block ID={}",
				source_block->ID, target_block->ID));
		}

		if (source_block->dataType != target_block->dataType)
		{
			throw std::runtime_error(std::format("source block data type={} not equal to target block data type={}",
				source_block->dataType, target_block->dataType));
		}

		if (source_block->dataType == SCR_CHAR_DATA)
		{
			const std::string key(source_block->c_dataPtr, source_block->dataSize - 1);
			if (IsLocalizedKey(key))
			{
				std::string_view value(target_block->c_dataPtr, target_block->dataSize - 1);
				_text.at(key).emplace(codepage.language_code, text_converter.Convert(value));
			}
		}

		if (source_block->nextLevel == nullptr)
		{
			if (target_block->nextLevel != nullptr)
			{
				throw std::runtime_error("source block next level not equal to target block next level");
			}
			return;
		}

		if (target_block->nextLevel == nullptr)
		{
			throw std::runtime_error("source block next level not equal to target block next level");
		}

		auto source_child = source_block->nextLevel->first();
		auto target_child = target_block->nextLevel->first();

		while (source_child != nullptr)
		{
			if (target_child == nullptr)
			{
				throw std::runtime_error("target child not found");
			}

			visitor(source_child, target_child);
			source_child = source_child->next;
			target_child = target_child->next;
		}
	};
	visitor(source_block, target_block);
}
