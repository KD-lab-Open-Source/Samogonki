#include "xerrhand.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

XErrorHandler ErrH;

// TODO
XErrorHandler::XErrorHandler() {}

XErrorHandler::~XErrorHandler() {}

void XErrorHandler::SetFlags(unsigned f) {}

void XErrorHandler::ClearFlags(unsigned f) {}

unsigned XErrorHandler::InfoFlags(unsigned f) { return 0; }

void XErrorHandler::SetPrefix(const char* s) {}

void XErrorHandler::SetPostfix(const char* s) {}

void XErrorHandler::SetRestore(void (*rf)()) {}

void XErrorHandler::Abort(const char* message, int code, int addval, const char* subj) {
  std::cout << "XErrorHandler: " << message << ", code " << code << ", addval " << addval;
  if (subj) {
    std::cout << ", subj: " << subj;
  }
  std::cout << std::endl;
  abort();
}

void XErrorHandler::Exit() { exit(0); }

void XErrorHandler::RTC(const char* file, unsigned int line, const char* expr) {
  std::stringstream message;
  message << file << ", LINE: " << line << std::endl << expr;
  Abort("RUN-TIME ERROR", XERR_USER, -1, message.str().c_str());
}

void XErrorHandler::WriteLog(char* error, char* msg) {}
