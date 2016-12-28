//-----------------------------------------------------------------------------
// z80asm global options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "memcheck.h"
#include "util.h"
#include <iostream>
#include <string>
#include <vector>

class Options {
public:
	Options();
	virtual ~Options();

	// search paths for files
	void add_include_path(const std::string& directory);
	void add_library_path(const std::string& directory);
	std::string search_include(const std::string& filename);
	std::string search_library(const std::string& filename);

	// streams for output
	std::ostream& cout() { return *cout_; }
	std::ostream& cerr() { return *cerr_; }
	void set_output(std::ostream& cout = std::cout, std::ostream& cerr = std::cerr) {
		cout_ = &cout; cerr_ = &cerr;
	}

	// command line options
	bool swap_ix_iy() const { return swap_ix_iy_; }
	void set_swap_ix_iy(bool swap_ix_iy) { swap_ix_iy_ = swap_ix_iy; }

private:
	std::vector<std::string> include_path_;
	std::vector<std::string> library_path_;
	std::ostream* cout_;		// information messages sink
	std::ostream* cerr_;		// error messages sink

	bool swap_ix_iy_;			// swap IX and IY instructions
};

extern Options opts;			// Global options

#endif //ndef OPTIONS_H_
