//-----------------------------------------------------------------------------
// z80asm - utilities
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef UTIL_H_
#define UTIL_H_

#include "memcheck.h"
#include <string>
#include <vector>
#include <fstream>

namespace util {

// getline function that handles all three line endings ("\r", "\n" and "\r\n"):
// http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
extern std::istream& getline(std::istream& is, std::string& t);

// spew/slurp files
extern void spew(const std::string& file, const std::string& text);
extern std::string slurp(const std::string& file);

// create and remove directory, return false and perror on failure; return true if directory exists/does not exist
extern bool mkdir(const std::string& directory);
extern bool rmdir(const std::string& directory);

// remove file, return false and perror on failure; return true if file does not exist
extern bool remove(const std::string& file);

// test for files
extern bool file_exists(const std::string& file);
extern bool is_file(const std::string& file);
extern bool is_dir(const std::string& file);
extern int  file_size(const std::string& file);		// -1 on error

// search for file in a list of directories
extern std::string file_search(const std::string& file, const std::vector<std::string>& dirs);

}; // namespace

#endif // UTIL_H_
