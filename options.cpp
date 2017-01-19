//-----------------------------------------------------------------------------
// z80asm global options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "options.h"
#include "message.h"
#include "util.h"
#include <iostream>
#include <fstream>

Options opts;			// Global options instance

Options::Options()
	: cout_(&std::cout)
	, cerr_(&std::cerr)
#define BOOL_OPT(NAME, OPT, OPT_ARG, HELP) \
	, NAME##_(false)
#define LIST_OPT(NAME, OPT, OPT_ARG, HELP)
#include "options.inc"
#undef BOOL_OPT
#undef LIST_OPT
{}

Options::~Options() {}

bool Options::check_opt_arg(const std::string& arg, const std::string& opt, std::string* out_opt_arg) {
	out_opt_arg->clear();
	if (arg.find(opt + "=") == 0) {
		*out_opt_arg = arg.substr(opt.size() + 1);
		return true;
	}
	else if (arg.find(opt) == 0) {
		*out_opt_arg = arg.substr(opt.size());
		return true;
	}
	else {
		return false;
	}
}

bool Options::parse_arg(const std::string& arg) {
	std::string opt_arg;

	if (arg.empty()) {
		// ignore empty args
	}
	else if (arg[0] == '@') {
		return parse_list(arg.substr(1));
	}

#define BOOL_OPT(NAME, OPT, OPT_ARG, HELP) \
	else if (arg == OPT) { \
		set_##NAME(true); \
	}
#define LIST_OPT(NAME, OPT, OPT_ARG, HELP) \
	else if (check_opt_arg(arg, OPT, &opt_arg)) { \
		if (opt_arg.empty()) { \
			err::missing_option_argument(arg); \
			return false; \
		} \
		else { \
			NAME(opt_arg); \
		} \
	}
#include "options.inc"
#undef BOOL_OPT
#undef LIST_OPT

	else if (arg[0] == '-') {
		err::unknown_option(arg);
		return false;
	}
	else {
		push_files(arg);
	}
	return true;
}

bool Options::parse_list(const std::string& filename) {
	std::ifstream ifs(filename.c_str());
	if (!ifs.good()) {
		std::perror(filename.c_str());
		return false;
	}

	std::string arg;
	while (ifs >> arg) {
		if (!parse_arg(arg))
			return false;
	}

	return true;
}

bool Options::parse(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (!parse_arg(argv[i]))
			return false;
	}

	// show help
	if (help()) {
		msg::help();
	}
	else if (files_.empty()) {
		msg::usage();
	}

	return true;
}

void Options::clear_files() {
	files_.clear();
}

void Options::push_files(const std::string& file) { 
	files_.push_back(file); 
}

Options::iterator Options::begin_files() {
	return files_.begin(); 
}

Options::iterator Options::end_files() {
	return files_.end(); 
}

void Options::clear_include_path() {
	include_path_.clear();
}

void Options::push_include_path(const std::string& directory) {
	include_path_.push_back(directory);
}

void Options::pop_include_path() {
	include_path_.pop_back();
}

Options::iterator Options::begin_include_path() {
	return include_path_.begin();
}

Options::iterator Options::end_include_path() {
	return include_path_.end();
}

std::string Options::search_include_path(const std::string& filename) {
	return util::file_search(filename, include_path_);
}

void Options::clear_library_path() {
	library_path_.clear();
}

void Options::push_library_path(const std::string& directory) {
	library_path_.push_back(directory);
}

void Options::pop_library_path() {
	library_path_.pop_back();
}

Options::iterator Options::begin_library_path() {
	return library_path_.begin();
}

Options::iterator Options::end_library_path() {
	return library_path_.end();
}

std::string Options::search_library_path(const std::string& filename) {
	return util::file_search(filename, library_path_);
}

std::ostream& Options::cout() { 
	return *cout_; 
}

std::ostream& Options::cerr() { 
	return *cerr_; 
}

void Options::set_out_err(std::ostream& cout, std::ostream& cerr) {
	cout_ = &cout; cerr_ = &cerr;
}
