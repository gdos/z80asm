//-----------------------------------------------------------------------------
// z80asm global options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "options.h"

Options opts;			// Global options instance

Options::Options() 
	: cout_(&std::cout), cerr_(&std::cerr)
	, swap_ix_iy_(false) {}

Options::~Options() {}

void Options::add_include_path(const std::string& directory) {
	include_path_.push_back(directory);
}

void Options::add_library_path(const std::string& directory) {
	library_path_.push_back(directory);
}

std::string Options::search_include(const std::string& filename) {
	return util::file_search(filename, include_path_);
}

std::string Options::search_library(const std::string& filename) {
	return util::file_search(filename, library_path_);
}
