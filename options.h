//-----------------------------------------------------------------------------
// z80asm global options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "memcheck.h"
#include <iostream>
#include <string>
#include <vector>

class Options {
public:
	typedef std::vector<std::string>::iterator iterator;

	Options();
	virtual ~Options();

	// parse command lines args, return false on error
	bool parse(int argc, char* argv[]);

	// arg files
	void clear_files();
	void push_files(const std::string& file);
	iterator begin_files();
	iterator end_files();

	// search paths
	void clear_include_path();
	void push_include_path(const std::string& directory);
	iterator begin_include_path();
	iterator end_include_path();
	std::string search_include_path(const std::string& filename);

	void clear_library_path();
	void push_library_path(const std::string& directory);
	iterator begin_library_path();
	iterator end_library_path();
	std::string search_library_path(const std::string& filename);

	// streams for output
	std::ostream& cout();
	std::ostream& cerr();
	void set_out_err(std::ostream& cout = std::cout, std::ostream& cerr = std::cerr);

private:
	std::vector<std::string> files_;		// input files
	std::vector<std::string> include_path_;	// search paths
	std::vector<std::string> library_path_;	// search paths
	std::ostream* cout_;					// information messages sink
	std::ostream* cerr_;					// error messages sink

	bool parse_arg(const std::string& arg);			// parse each option
	bool parse_list(const std::string& filename);	// parse '@lists'
	bool check_opt_arg(const std::string& arg, const std::string& opt, std::string* out_opt_arg);

	// command line options
#define BOOL_OPT(NAME, OPT, OPT_ARG, HELP) \
public: \
	bool NAME() const { return NAME##_; } \
	void set_##NAME(bool NAME) { NAME##_ = NAME; } \
private: \
	bool NAME##_;
#define LIST_OPT(NAME, OPT, OPT_ARG, HELP)
#include "options.inc"
#undef BOOL_OPT
#undef LIST_OPT

};

extern Options opts;			// Global options

#endif //ndef OPTIONS_H_
