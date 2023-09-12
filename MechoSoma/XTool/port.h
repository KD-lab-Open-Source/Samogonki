#ifndef __PORT_H
#define __PORT_H

#include <cmath>
#include <cstdint>

/**
	
 * C++ version char* style "itoa":
	
 */
	
const char* port_itoa( int value, char* result, int base );

const char* port_ltoa( long value, char* result, int base );

const char* port_ultoa( unsigned long value, char* result, int base );

#if (defined(__unix__) || defined(__APPLE__)) && !defined(EMSCRIPTEN)
char *strupr(char *string);
#endif

// Copied C version from Vangers
// TODO(amdmi3): very inefficient; rewrite
inline int BitSR(int x) {
	unsigned int a = abs(x);
	for (int i = 31; i > 0; i--)
		if (a & (1 << i))
			return i;

	return 0;
}

#ifndef _WIN32
char *_fullpath(char *absPath, const char *relPath, size_t maxLength);
#endif

uint32_t GetPrivateProfileString(
	const char* lpAppName,
	const char* lpKeyName,
	const char* lpDefault,
	char* lpReturnedString,
	uint32_t nSize,
	const char* lpFileName
);

bool WritePrivateProfileString(
	const char* lpAppName,
	const char* lpKeyName,
	const char* lpString,
	const char* lpFileName
);

#endif
