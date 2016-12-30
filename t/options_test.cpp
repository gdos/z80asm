//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "options.h"
#include "util.h"
#include "z80asm_config.h"
#include "test.h"
#include <iostream>
#include <sstream>

void test_files() {
	Options::iterator it;
	it = opts.begin_files();
	OK(it == opts.end_files());

	opts.push_files("f1.asm");
	opts.push_files("f2.asm");

	it = opts.begin_files();
	IS(*it, "f1.asm"); ++it;
	IS(*it, "f2.asm"); ++it;
	OK(it == opts.end_files());

	opts.clear_files();
	it = opts.begin_files();
	OK(it == opts.end_files());
}

void test_paths() {
	Options::iterator it;

	util::mkdir("tmp1");
	create_test_file("tmp1/path.h", "");

	util::mkdir("tmp2");
	create_test_file("tmp2/path.lib", "");

	IS(opts.search_include_path("path.h"), "path.h");
	opts.push_include_path("tmp1");
	IS(opts.search_include_path("path.h"), "tmp1/path.h");
	
	it = opts.begin_include_path();
	IS(*it, "tmp1"); ++it;
	OK(it == opts.end_include_path());

	opts.clear_include_path();
	IS(opts.search_include_path("path.h"), "path.h");

	IS(opts.search_library_path("path.lib"), "path.lib");
	opts.push_library_path("tmp2");
	IS(opts.search_library_path("path.lib"), "tmp2/path.lib");

	it = opts.begin_library_path();
	IS(*it, "tmp2");
	++it;
	OK(it == opts.end_library_path());

	opts.clear_library_path();
	IS(opts.search_library_path("path.lib"), "path.lib");

	delete_test_file("tmp1/path.h");
	delete_test_file("tmp2/path.lib");
	util::rmdir("tmp1");
	util::rmdir("tmp2");
}

void test_redirect() {
	DIAG("Test stdout");
	opts.cout() << "Test stdout" << std::endl;
	DIAG("Test stderr");
	opts.cerr() << "Test stderr" << std::endl;
	
	std::ostringstream strout, strerr;
	opts.set_out_err(strout, strerr);	// redirect
	opts.cout() << "Test stdout" << std::endl;
	opts.cerr() << "Test stderr" << std::endl;

	IS(strout.str(), "Test stdout\n");
	IS(strerr.str(), "Test stderr\n");

	opts.set_out_err();					// back to default
	DIAG("Test stdout");
	opts.cout() << "Test stdout" << std::endl;
	DIAG("Test stderr");
	opts.cerr() << "Test stderr" << std::endl;
}

#define T_OPT(NAME) \
	OK(opts.NAME() == false); \
	opts.set_##NAME(true); \
	OK(opts.NAME() == true); \
	opts.set_##NAME(false); \
	OK(opts.NAME() == false)

void test_options() {
	T_OPT(help);
	T_OPT(verbose);
	T_OPT(do_list);
	T_OPT(swap_ix_iy);
}

// stop gcc complaining about initializing a non-const char*[] from a literal string
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

#define _ ,
#define T_PARSE(argv_, ret_, strout_, strerr_) \
	{ \
		std::ostringstream strout, strerr; \
		opts.clear_files(); \
		opts.clear_include_path(); \
		opts.clear_library_path(); \
		opts.set_out_err(strout, strerr); \
		char* argv[] = { Z80ASM_PROG, argv_ }; \
		int argc = 0; while ( argv[argc] != NULL) argc++; \
		OK(opts.parse(argc, argv) == ret_); \
		IS(strout.str(), strout_); \
		IS(strerr.str(), strerr_); \
		opts.set_out_err(); \
	}

void test_parse() {	
	Options::iterator it;

	T_PARSE(NULL, 
		true,
		Z80ASM_NAME " " Z80ASM_VERSION " " Z80ASM_COPYRIGHT "\n"
		"\n"
		"Usage: z80asm [options] {FILES|@FILELIST}...\n",
		"");

	T_PARSE("" _ NULL,
		true,
		Z80ASM_NAME " " Z80ASM_VERSION " " Z80ASM_COPYRIGHT "\n"
		"\n"
		"Usage: z80asm [options] {FILES|@FILELIST}...\n",
		"");

	T_PARSE("" _ "-h" _ NULL,
		true,
		Z80ASM_NAME " " Z80ASM_VERSION " " Z80ASM_COPYRIGHT "\n"
		"\n"
		"Usage: z80asm [options] {FILES|@FILELIST}...\n"
		"\n"
		"Options:\n"
		"  -h            Show help options\n"
		"  -v            Be verbose\n"
		"  -I=PATH       Add directory to include search path\n"
		"  -L=PATH       Add directory to library search path\n"
		"  -l            Create listing file.lis\n"
		"  --IXIY        Swap IX and IY registers\n",
		"");
	IS(opts.help(), true); opts.set_help(false);

	T_PARSE("-v" _ "f1.asm" _ "" _ "f2.asm" _ NULL, true, "", "");
	IS(opts.verbose(), true); opts.set_verbose(false);
	it = opts.begin_files(); 
	IS(*it, "f1.asm"); ++it;
	IS(*it, "f2.asm"); ++it;
	OK(it == opts.end_files());

	T_PARSE("" _ "f1.asm" _ "" _ "f2.asm" _ "-v" _ NULL, true, "", "");
	IS(opts.verbose(), true); opts.set_verbose(false);
	it = opts.begin_files();
	IS(*it, "f1.asm"); ++it;
	IS(*it, "f2.asm"); ++it;
	OK(it == opts.end_files());

	T_PARSE("-l" _ "f1.asm" _ NULL, true, "", "");
	IS(opts.do_list(), true); opts.set_do_list(false);
	it = opts.begin_files();
	IS(*it, "f1.asm"); ++it;
	OK(it == opts.end_files());

	T_PARSE("--IXIY" _ "f1.asm" _ NULL, true, "", "");
	IS(opts.swap_ix_iy(), true); opts.set_swap_ix_iy(false);
	it = opts.begin_files();
	IS(*it, "f1.asm"); ++it;
	OK(it == opts.end_files());

	T_PARSE("-vxx" _ NULL, false, "", "Error: -vxx: unknown option, use -h for usage\n");

	// @lists
	create_test_file("test1.lst", " f1.asm\n@test2.lst");
	create_test_file("test2.lst", "-v f2.asm\n-l");

	T_PARSE("@test1.lst" _ NULL, true, "", "");
	IS(opts.verbose(), true); opts.set_verbose(false);
	IS(opts.do_list(), true); opts.set_do_list(false);
	it = opts.begin_files();
	IS(*it, "f1.asm"); ++it;
	IS(*it, "f2.asm"); ++it;
	OK(it == opts.end_files());

	delete_test_file("test1.lst");
	delete_test_file("test2.lst");

	create_test_file("test1.lst", " f1.asm\n@test2.lst");
	T_PARSE("@test1.lst" _ NULL, false, "", "");		// perror
	delete_test_file("test1.lst");

	// lists
	T_PARSE("-Idir1" _ "-I=dir2" _ "f1.asm" _ NULL, true, "", "");
	it = opts.begin_include_path();
	IS(*it, "dir1"); ++it;
	IS(*it, "dir2"); ++it;
	OK(it == opts.end_include_path());

	T_PARSE("-Ldir1" _ "-L=dir2" _ "f1.asm" _ NULL, true, "", "");
	it = opts.begin_library_path();
	IS(*it, "dir1"); ++it;
	IS(*it, "dir2"); ++it;
	OK(it == opts.end_library_path());

	T_PARSE("-I" _ "f1.asm" _ NULL, false, "", "Error: -I: missing option argument, use -h for usage\n");
	T_PARSE("-L=" _ "f1.asm" _ NULL, false, "", "Error: -L=: missing option argument, use -h for usage\n");
}

int main() {
	START_TESTING();
	test_files();
	test_paths();
	test_redirect();
	test_options();
	test_parse();
	DONE_TESTING();
}
