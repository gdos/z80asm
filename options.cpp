//-----------------------------------------------------------------------------
// z80asm options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#include "options.h"

Options::Options() {
}

Options::~Options() {
}

void Options::add_include_path(const std::string& directory) {
	include_path_.append(directory);
}

void Options::add_library_path(const std::string& directory) {
	library_path_.append(directory);
}

std::string Options::search_include(const std::string& filename) {
	filesystem::path filepath(filename);
	filesystem::path fullpath = include_path_.resolve(filepath);
	return fullpath.str();
}

std::string Options::search_library(const std::string& filename) {
	filesystem::path filepath(filename);
	filesystem::path fullpath = library_path_.resolve(filepath);
	return fullpath.str();
}

