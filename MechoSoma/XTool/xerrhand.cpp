#include <cstdlib>
#include <iostream>

#include "xtool.h"

XErrorHandler ErrH;

// TODO
XErrorHandler::XErrorHandler(void) {}

XErrorHandler::~XErrorHandler(void) {}

void XErrorHandler::SetFlags(unsigned f) {}

void XErrorHandler::ClearFlags(unsigned f) {}

unsigned XErrorHandler::InfoFlags(unsigned f) { return 0; }

void XErrorHandler::SetPrefix(const char* s) {}

void XErrorHandler::SetPostfix(const char* s) {}

void XErrorHandler::SetRestore(void (*rf)(void)) {}

void XErrorHandler::Abort(const char* message, int code, int addval, const char* subj) {
  std::cout << "XErrorHandler: " << message << ", code " << code << ", addval " << addval;
  if (subj) {
    std::cout << ", subj: " << subj;
  }
  std::cout << std::endl;
  abort();
}

void XErrorHandler::Exit(void) { exit(0); }

void XErrorHandler::RTC(const char* file, unsigned int line, const char* expr) {}

void XErrorHandler::WriteLog(char* error, char* msg) {}
