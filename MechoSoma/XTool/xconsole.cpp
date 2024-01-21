#include <iostream>

#include "xtool.h"

XConsole XCon;

XConsole::XConsole(void) {}

XConsole::~XConsole(void) {}

XConsole& XConsole::operator<(const char* msg) {
#ifdef _DEBUG
  std::cout << msg << std::endl;
#endif
  return *this;
}

XConsole& XConsole::operator<= (int value) {
#ifdef _DEBUG
  std::cout << "(int) " << value << std::endl;
#endif
  return *this;
}

XConsole& XConsole::operator<= (float value) {
#ifdef _DEBUG
  std::cout << "(float) " << value << std::endl;
#endif
  return *this;
}
