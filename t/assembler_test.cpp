//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "assembler.h"
#include "options.h"
#include "util.h"
#include "test.h"
#include <iostream>
#include <sstream>

#define T_STR(a, b) { \
		std::string sa(a); \
		std::string sb(b); \
		OK(sa == sb); \
		if (sa != sb) { DIAG("<<<"); DIAG(sa); DIAG(">>>"); DIAG(sb); DIAG("---"); } \
	}

#define T_ASM_1(file_, expected_ok_, expected_out_, expected_err_) \
	{ \
		as->clear(); \
		OK(as->good()); \
		std::ostringstream strout, strerr; \
		opts.set_out_err(strout, strerr); \
		IS(as->assemble(file_), expected_ok_); \
		IS(as->good(), expected_ok_); \
		T_STR(expected_out_, strout.str()); \
		T_STR(expected_err_, strerr.str()); \
		opts.set_out_err(); \
	}

#define T_ASM(file_, expected_ok_, expected_out_, expected_err_) \
	for (int i = 0; i < 2; i++) { \
		bool verbose = i > 0; \
		opts.set_verbose(verbose); \
		T_ASM_1(file_, expected_ok_, verbose ? expected_out_ : "", expected_err_); \
		opts.set_verbose(false); \
	}

void test_include() {
	Assembler* as;

	create_test_file("test.asm", "\tINCLUDE\n");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n",
		"test.asm:1: Error: expected file\n"
		"\t\tINCLUDE\n"
		"\t\t       ^~~~\n"
	);
	delete as;

	create_test_file("test.asm", "\tinclude ;comment\n");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n",
		"test.asm:1: Error: expected file\n"
		"\t\tinclude ;comment\n"
		"\t\t        ^~~~\n"
	);
	delete as;

	create_test_file("test.asm", "include\t\" ;comment\n");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n",
		"test.asm:1: Error: expected quotes\n"
		"\tinclude\t\" ;comment\n"
		"\t       \t          ^~~~\n"
	);
	delete as;

	create_test_file("test.asm", "include \"unexistant.asm\" ;comment\n");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n",
		"test.asm:1: Error: unexistant.asm: open failure\n"
		"\tinclude \"unexistant.asm\" ;comment\n"
		"\t                        ^~~~\n"
	);
	delete as;

	create_test_file("test1.asm", "");
	create_test_file("test.asm", "include \"test1.asm\" ;comment\n");
	as = new Assembler;
	T_ASM("test.asm", true,
		"Assembling test.asm\n"
		"Reading test.asm\n"
		"Reading test1.asm\n",
		""
	);
	delete as;

	create_test_file("test.asm", "include \"""\" ;comment\n");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n",
		"test.asm:1: Error: : open failure\n"
		"\tinclude \"""\" ;comment\n"
		"\t          ^~~~\n"
	);
	delete as;

	create_test_file("test1.asm", "");
	create_test_file("test2.asm", "");
	create_test_file("test.asm", "include \"test1.asm\"\\include \"test2.asm\"\n");
	as = new Assembler;
	T_ASM("test.asm", true,
		"Assembling test.asm\n"
		"Reading test.asm\n"
		"Reading test1.asm\n"
		"Reading test2.asm\n",
		""
	);
	delete as;

	create_test_file("test1.asm", "");
	create_test_file("test.asm", "include \"test1.asm\"nop\n");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n"
		"Reading test1.asm\n",
		"test.asm:1: Error: expected end of statement\n"
		"\tinclude \"test1.asm\"nop\n"
		"\t                   ^~~~\n"
	);
	delete as;

	create_test_file("test1.asm", "include \"test2.asm\"");
	create_test_file("test2.asm", "include \"test1.asm\"");
	create_test_file("test.asm", "include \"test1.asm\"");
	as = new Assembler;
	T_ASM("test.asm", false,
		"Assembling test.asm\n"
		"Reading test.asm\n"
		"Reading test1.asm\n"
		"Reading test2.asm\n",
		"test2.asm:1: Error: test1.asm: recursive include\n"
		"\tinclude \"test1.asm\"\n"
		"\t                   ^~~~\n"
	);
	delete as;

	// check that source dir is added to include path
	util::mkdir("test_dir");
	create_test_file("test_dir/test1.asm", "");
	create_test_file("test_dir/test.asm", "include \"test1.asm\"");
	delete_test_file("test1.asm");
	as = new Assembler;
	T_ASM("test_dir/test.asm", true,
		"Assembling test_dir/test.asm\n"
		"Reading test_dir/test.asm\n"
		"Reading test_dir/test1.asm\n",
		""
	);
	delete as;

	delete_test_file("test_dir/test1.asm");
	delete_test_file("test_dir/test.asm");

	// check include with path
	create_test_file("test_dir/test1.asm", "");
	create_test_file("test.asm", "include \"test_dir/test1.asm\"");
	delete_test_file("test1.asm");
	as = new Assembler;
	T_ASM("test.asm", true,
		"Assembling test.asm\n"
		"Reading test.asm\n"
		"Reading test_dir/test1.asm\n",
		""
	);
	delete as;

	delete_test_file("test_dir/test1.asm");
	delete_test_file("test.asm");

	delete_test_file("test.asm");
	delete_test_file("test1.asm");
	delete_test_file("test2.asm");

	util::rmdir("test_dir");
}

void test_open() {
	Assembler* as;

	delete_test_file("unexistant.asm");
	as = new Assembler;
	DIAG("Open file error");
	T_ASM("unexistant.asm", false, "Assembling unexistant.asm\n", "Error: unexistant.asm: open failure\n");
	delete as;

	delete_test_file("test.asm");
	util::mkdir("test_dir");
	create_test_file("test_dir/test.asm", "");

	as = new Assembler;
	DIAG("Open file error");
	T_ASM("test.asm", false, "Assembling test.asm\n", "Error: test.asm: open failure\n");
	delete as;

	opts.push_include_path("test_dir");
	as = new Assembler;
	T_ASM("test.asm", true,
		"Assembling test.asm\n"
		"Reading test_dir/test.asm\n", "");
	delete as;
	
	delete_test_file("test_dir/test.asm");
	delete_test_file("test.asm");

	util::rmdir("test_dir");
	delete_test_file("test.asm");
}

void test_opcodes() {
	Assembler* as;

	create_test_file("test.asm", "nop\\nop\n nop;nop\n nop \\ nop\n");
	as = new Assembler;
	T_ASM("test.asm", true,
		"Assembling test.asm\n"
		"Reading test.asm\n", "");
	delete as;
	delete_test_file("test.asm");
}

int main() {
	START_TESTING();

	test_open();
	test_include();
	test_opcodes();

	DONE_TESTING();
}
