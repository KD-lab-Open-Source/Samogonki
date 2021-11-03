#ifndef __PORT_H
#define __PORT_H

#include <cmath>

/**
	
 * C++ version char* style "itoa":
	
 */
	
char* port_itoa( int value, char* result, int base );

char* port_ltoa( long value, char* result, int base );

char* port_ultoa( unsigned long value, char* result, int base );

#if defined(__unix__) || defined(__APPLE__)
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

char *_fullpath(char *absPath, const char *relPath, size_t maxLength);

char *strlwr(char *str);

#endif
