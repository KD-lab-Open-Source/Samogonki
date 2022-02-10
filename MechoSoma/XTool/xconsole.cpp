#include <iostream>

#include "xtool.h"

XConsole XCon;

XConsole::XConsole(void) {}

XConsole::~XConsole(void) {}

XConsole& XConsole::operator<(const char* msg) {
  std::cout << msg << std::endl;
  return *this;
}

XConsole& XConsole::operator<= (int value) {
  std::cout << "(int) " << value << std::endl;
  return *this;
}

XConsole& XConsole::operator<= (float value) {
  std::cout << "(float) " << value << std::endl;
  return *this;
}
