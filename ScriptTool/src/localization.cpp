#include "localization.h"

#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "common.h"
#include "text_converter.h"

namespace
{

class PropertyReader final
{
public:
	explicit PropertyReader(std::string_view buffer) : _buffer(buffer) {}

	std::optional<std::string> read_value(std::string_view key)
	{
		if (_state == State::error)
		{
			return nullptr;
		}

		if (!skip_tag(key))
		{
			_state = State::error;
			return std::nullopt;
		}

		const auto end = _buffer.find(tag);
		if (end == std::string_view::npos)
		{
			if (!_buffer.ends_with(line_break))
			{
				_state = State::error;
				return std::nullopt;
			}
			_buffer.remove_suffix(line_break.size());

			return std::string(_buffer);
		}

		if (end == 0)
		{
			_state = State::error;
			return std::nullopt;
		}

		auto result = _buffer.substr(0, end);
		if (!result.ends_with(line_break))
		{
			_state = State::error;
			return std::nullopt;
		}
		result.remove_suffix(line_break.size());

		_buffer.remove_prefix(end);
		_buffer.remove_prefix(tag.size());

		return std::string(result);
	}

private:
	bool skip_tag(std::string_view key)
	{
		if (_state == State::stopped)
		{
			if (!_buffer.starts_with(tag))
			{
				return false;
			}

			_state = State::started;
			_buffer.remove_prefix(tag.size());
		}
		
		if (!_buffer.starts_with(key))
		{
			return false;
		}
		_buffer.remove_prefix(key.size());

		if (!_buffer.starts_with(line_break))
		{
			return false;
		}
		_buffer.remove_prefix(line_break.size());

		return true;
	}

	static constexpr std::string_view tag{"$$$"};
	static constexpr std::string_view line_break{"\r\n"};

	std::string_view _buffer;

	enum class State
	{
		stopped, started, error
	};
	State _state = State::stopped;
};

}

using namespace text;

void Localization::Load(const fs::path &file_path)
{
	const auto buffer = common::load_file(file_path);

	struct LanguageData final
	{
		explicit LanguageData(const common::Codepage &codepage)
			: short_name(codepage.language_code), converter("utf-8", codepage.codepage) {}
	
		const std::string_view short_name;
		text::TextConverter converter;
	};
	std::array<LanguageData, common::codepages.size()> language_data{
		LanguageData{common::codepages[0]},
		LanguageData{common::codepages[1]},
		LanguageData{common::codepages[2]},
		LanguageData{common::codepages[3]},
		LanguageData{common::codepages[4]},
		LanguageData{common::codepages[5]}
	};

	PropertyReader reader(buffer);
	for (;;)
	{
		const auto key = reader.read_value("ID");
		if (!key)
		{
			break;
		}

		for (auto &data : language_data)
		{
			const auto language = reader.read_value("LANGUAGE");
			if (!language)
			{
				throw std::runtime_error("language property expected");
			}
			if (*language != data.short_name)
			{
				throw std::runtime_error(std::format("expected language={}", data.short_name));
			}

			const auto text = reader.read_value("TEXT");
			if (!text)
			{
				throw std::runtime_error("text property expected");
			}

			_text[*key].emplace(data.short_name, std::format("\"{}\"", data.converter.Convert(*text)));
		}
	}
}

void Localization::SetLanguage(const std::string_view language)
{
	_language = language;
}

const std::string_view Localization::GetLocalizedKey(const std::string_view key) const
{
	const auto p = std::find_if(_text.cbegin(), _text.cend(), [&key](const auto &e) { return e.first == key; });
	if (p == _text.end())
	{
		return "";
	}

	const auto text = std::find_if(
		p->second.cbegin(),
		p->second.cend(),
		[this](const auto &e) { return e.first == _language; }
	);
	if (text == p->second.end())
	{
		throw std::runtime_error(std::format("unknown language={}", _language));
	}

	return text->second;
}
