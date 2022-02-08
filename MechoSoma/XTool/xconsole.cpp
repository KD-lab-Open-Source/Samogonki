#include "xtool.h"

XConsole XCon;

XConsole::XConsole(void)
{
}

XConsole::~XConsole(void)
{
}

XConsole& XConsole::operator< (const char*)
{
	return *this;
}
