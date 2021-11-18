#include "xtool.h"

#include <cstdlib>

XErrorHandler ErrH;

// TODO
XErrorHandler::XErrorHandler(void)
{
}

XErrorHandler::~XErrorHandler(void)
{
}

void XErrorHandler::SetFlags(unsigned f)
{
}

void XErrorHandler::ClearFlags(unsigned f)
{
}

unsigned XErrorHandler::InfoFlags(unsigned f)
{
}

void XErrorHandler::SetPrefix(const char* s)
{
}

void XErrorHandler::SetPostfix(const char* s)
{
}

void XErrorHandler::SetRestore(void (*rf)(void))
{
}

void XErrorHandler::Abort(const char* message, int code, int addval, const char* subj)
{
	abort();
}

void XErrorHandler::Exit(void)
{
	exit(0);
}

void XErrorHandler::RTC(const char *file,unsigned int line, const char *expr)
{
}

void XErrorHandler::WriteLog(char* error, char* msg)
{
}
