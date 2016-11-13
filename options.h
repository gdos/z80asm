//-----------------------------------------------------------------------------
// z80asm options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "filesystem/resolver.h"
#include <string>

class Options {
public:
	Options();
	virtual ~Options();
	
	// paths for files
	void add_include_path(const std::string& directory);
	void add_library_path(const std::string& directory);
	std::string search_include(const std::string& filename);
	std::string search_library(const std::string& filename);

private:
	filesystem::resolver include_path_;
	filesystem::resolver library_path_;
};

#endif // ndef OPTIONS_H_
