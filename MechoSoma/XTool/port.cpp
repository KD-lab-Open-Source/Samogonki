#include "port.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include <filesystem>
#include "filesystem.h"
#include "xtool.h"
#include "xINI_File.h"

char *strupr(char *string) {
      char *s;

      if (string) {
            for (s = string; *s; ++s)
                  *s = toupper(*s);
      }
      return string;
}

/**

 * C++ version char* style "itoa":

 */

char* port_itoa( int value, char* result, int base ) {
	// check that the base if valid
	if (base < 2 || base > 16) {
		*result = 0;
		return result;
	}

	char* out = result;
	int quotient = value;

	do {
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
		++out;
		quotient /= base;

	} while ( quotient );

	// Only apply negative sign for base 10
	if ( value < 0 && base == 10)
		*out++ = '-';

	std::reverse( result, out );
	*out = 0;
	return result;
}

char* port_ltoa( long value, char* result, int base ) {
	// check that the base if valid
	if (base < 2 || base > 16) {
		*result = 0;
		return result;
		}

	char* out = result;
	long quotient = value;

	do {
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
		++out;
		quotient /= base;
	} while ( quotient );

	// Only apply negative sign for base 10
	if ( value < 0 && base == 10)
		*out++ = '-';

	std::reverse( result, out );
	*out = 0;
	return result;
}

char* port_ultoa( unsigned long value, char* result, int base ) {
	// check that the base if valid
	if (base < 2 || base > 16) {
		*result = 0;
		return result;
	}

	char* out = result;
	unsigned long quotient = value;

	do {
		*out = "0123456789abcdef"[ quotient % base ];
		++out;
		quotient /= base;
	} while ( quotient );

	std::reverse( result, out );
	*out = 0;
	return result;
}

#ifndef _WIN32
char *_fullpath(char *absPath, const char *relPath, size_t maxLength)
{
	return realpath(relPath, absPath);
}

char *strlwr(char *str)
{
	return str;
}
#endif

struct PrivateProfile
{
	std::string name;
	xINI_File* file = nullptr;

	explicit PrivateProfile(const char* name)
	{
		this->name = name;
		file = new xINI_File(name);
		file->load();
	}

	~PrivateProfile()
	{
		delete file;
	}
};
std::vector<std::unique_ptr<PrivateProfile>> private_profiles;

uint32_t GetPrivateProfileString(
	const char* lpAppName,
	const char* lpKeyName,
	const char* lpDefault,
	char* lpReturnedString,
	uint32_t nSize,
	const char* lpFileName
)
{
	xINI_File* file;

	auto p = std::find_if(private_profiles.cbegin(), private_profiles.cend(), [lpFileName](const auto& e) {
		return e->name == lpFileName;
	});
	if (p == private_profiles.cend())
	{
		const auto path = file::normalize_path(lpFileName);
		if (!std::filesystem::exists(path))
		{
			return 0;
		}
		file = private_profiles.emplace_back(std::make_unique<PrivateProfile>(lpFileName))->file;
	}
	else
	{
		file = p->get()->file;
	}

	auto value = file->get_key(lpAppName, lpKeyName);
	if (value == nullptr)
	{
		return 0;
	}

	std::string t(value);
	const size_t value_size = std::min(t.size(), static_cast<size_t>(nSize));
	std::copy(t.cbegin(), t.cbegin() + value_size, lpReturnedString);
	lpReturnedString[value_size] = 0;

	return value_size;
}

bool WritePrivateProfileString(
	const char* lpAppName,
	const char* lpKeyName,
	const char* lpString,
	const char* lpFileName
)
{
	return false;
}
