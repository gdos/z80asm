//-----------------------------------------------------------------------------
// z80asm user messages
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "message.h"
#include "options.h"
#include "z80asm_config.h"

namespace msg {
	void version() {
		opts.cout() << Z80ASM_NAME << " " << Z80ASM_VERSION << " " << Z80ASM_COPYRIGHT << std::endl;
	}

	void usage() {
		version();
		opts.cout() << std::endl
			<< "Usage: z80asm [options] {FILES|@FILELIST}..." << std::endl;
	}

	void help() {
		usage();
		opts.cout() << std::endl
			<< "Options:" << std::endl
#define BOOL_OPT(NAME, OPT, OPT_ARG, HELP) \
			<< "  " << OPT << "    " << HELP << std::endl
#define LIST_OPT(NAME, OPT, OPT_ARG, HELP) \
			<< "  " << OPT << OPT_ARG << "    " << HELP << std::endl
#include "options.inc"
#undef BOOL_OPT
#undef LIST_OPT
			;
	}

	void assembling(const std::string& file) {
		opts.cout() << "Assembling " << file << std::endl;
	}

}; // namespace

namespace err {
	void unknown_option(const std::string& arg) {
		opts.cerr() << "Error: " << arg << ": unknown option, use -h for usage" << std::endl;
	}

	void missing_option_argument(const std::string& arg) {
		opts.cerr() << "Error: " << arg << ": missing option argument, use -h for usage" << std::endl;
	}

}; // namespace
