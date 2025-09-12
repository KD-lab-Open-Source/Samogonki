#include "text_converter.h"

#include <algorithm>
#include <cassert>

using namespace text;

TextConverter::TextConverter(const char *from_codepage, const char *to_codepage)
{
	_descriptor = iconv_open(to_codepage, from_codepage);
	assert(_descriptor != (iconv_t) -1);

	_string_buffer.resize(8192, 0);
}

TextConverter::~TextConverter()
{
	iconv_close(_descriptor);
}

std::string TextConverter::Convert(std::string_view input)
{
	std::string t(input);
	char *input_data = t.data();
	size_t input_length = t.size();

	std::fill(_string_buffer.begin(), _string_buffer.end(), 0);
	char *output_data = _string_buffer.data();
	size_t output_length = _string_buffer.size() - 1;

	iconv(_descriptor, &input_data, &input_length, &output_data, &output_length);
	assert(input_length == 0);

	return std::string(_string_buffer.data(), output_data);
}
