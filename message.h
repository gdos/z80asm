//-----------------------------------------------------------------------------
// z80asm user messages
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "fwd.h"
#include <string>

namespace msg {
	extern void version();
	extern void usage();
	extern void help();
	extern void assembling(const std::string& file);
	extern void loading(const std::string& file);
	extern void reading(const std::string& file);
}; // namespace

namespace err {
	extern void unknown_option(const std::string& arg);
	extern void missing_option_argument(const std::string& arg);
	extern void recursive_include(const std::string& file, SrcLine* from = NULL, int column = 0);
	extern void open_file(const std::string& file, SrcLine* from = NULL, int column = 0);
	extern void syntax(SrcLine* from = NULL, int column = 0);
	extern void expected_file(SrcLine* from = NULL, int column = 0);
	extern void expected_quotes(SrcLine* from = NULL, int column = 0);
	extern void expected_eos(SrcLine* from = NULL, int column = 0);
	extern void missing_closing_quote(SrcLine* from = NULL, int column = 0);
	extern void missing_closing_bracket(SrcLine* from = NULL, int column = 0);
	extern void failure(SrcLine* from = NULL);
}; // namespace

#endif // MESSAGE_H_
