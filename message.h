//-----------------------------------------------------------------------------
// z80asm user messages
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

namespace msg {
	extern void version();
	extern void usage();
	extern void help();
	extern void assembling(const std::string& file);
}; // namespace

namespace err {
	extern void unknown_option(const std::string& arg);
	extern void missing_option_argument(const std::string& arg);

}; // namespace

#endif // MESSAGE_H_
