//-----------------------------------------------------------------------------
// z80asm - utilities
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <fstream>

// getline function that handles all three line endings ("\r", "\n" and "\r\n"):
// http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
extern std::istream& safe_getline(std::istream& is, std::string& t);

#endif // UTIL_H_
