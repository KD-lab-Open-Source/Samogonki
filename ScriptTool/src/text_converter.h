#pragma once

#include <string>
#include <string_view>

#include <iconv.h>

namespace text
{

class TextConverter final
{
public:
	TextConverter(const char *from_codepage, const char *to_codepage);
	~TextConverter();
	std::string Convert(std::string_view input);

private:
	iconv_t _descriptor;
	std::string _string_buffer;
};

}
