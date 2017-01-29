//-----------------------------------------------------------------------------
// z80asm user messages
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "message.h"
#include "options.h"
#include "source.h"
#include "z80asm_config.h"
#include <algorithm>

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

	void loading(const std::string& file) {
		opts.cout() << "Loading " << file << std::endl;
	}

	void reading(const std::string& file) {
		opts.cout() << "Reading " << file << std::endl;
	}

}; // namespace

namespace err {
	static void message_at(SrcLine* from, int column, bool is_error,
		const std::string& m1, const std::string& m2 = "", const std::string& m3 = "") {
		if (from && from->src_file())
			opts.cerr() << from->src_file()->filename() << ':';
		if (from)
			opts.cerr() << from->line_nr() << ": ";
		if (is_error)
			opts.cerr() << "Error";
		else
			opts.cerr() << "Warning";
		opts.cerr() << ": " << m1 << m2 << m3 << std::endl;
		if (from && column > 0) {
			std::string text = from->text();
			std::replace(text.begin(), text.end(), '\n', '\\');
			opts.cerr() << "\t" << text << std::endl << "\t";

			for (int i = 1; i < column; i++) {
				char c;
				if (i - 1 < static_cast<int>(text.length()))
					c = text[i - 1];
				else
					c = ' ';

				if (c == '\t')
					opts.cerr() << c;
				else
					opts.cerr() << ' ';
			}
			opts.cerr() << "^~~~" << std::endl;
		}
	}

	static void error_at(SrcLine* from, int column,
		const std::string& m1, const std::string& m2 = "", const std::string& m3 = "") {
		message_at(from, column, true, m1, m2, m3);
	}

	static void warning_at(SrcLine* from, int column,
		const std::string& m1, const std::string& m2 = "", const std::string& m3 = "") {
		message_at(from, column, false, m1, m2, m3);
	}

	void unknown_option(const std::string& arg) {
		error_at(NULL, 0, arg, ": unknown option, use -h for usage");
	}

	void missing_option_argument(const std::string& arg) {
		error_at(NULL, 0, arg, ": missing option argument, use -h for usage");
	}

	void recursive_include(const std::string& file, SrcLine* from, int column) {
		error_at(from, column, file, ": recursive include");
	}

	void open_file(const std::string& file, SrcLine* from, int column) {
		error_at(from, column, file, ": open failure");
	}

	void syntax(SrcLine* from, int column) {
		error_at(from, column, "syntax");
	}

	void expected_file(SrcLine* from, int column) {
		error_at(from, column, "expected file");
	}

	void expected_quotes(SrcLine* from, int column) {
		error_at(from, column, "expected quotes");
	}

	void expected_end_of_statement(SrcLine* from, int column) {
		error_at(from, column, "expected end of statement");
	}

	void missing_closing_quote(SrcLine* from, int column) {
		error_at(from, column, "missing closing quote");
	}

	void missing_closing_bracket(SrcLine* from, int column) {
		error_at(from, column, "missing closing bracket");
	}

	void squoted_string(SrcLine* from, int column) {
		error_at(from, column, "invalid single quoted character");
	}

	void failure(SrcLine* from) {
		error_at(from, 0, "parse failure");
	}
}; // namespace
