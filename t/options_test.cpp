//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "options.h"
#include "util.h"
#include "test.h"
#include <iostream>
#include <sstream>

void test_paths() {
	util::mkdir("tmp1");
	create_test_file("tmp1/path.h", "");

	util::mkdir("tmp2");
	create_test_file("tmp2/path.lib", "");

	IS(opts.search_include("path.h"), "path.h");
	opts.add_include_path("tmp1");
	IS(opts.search_include("path.h"), "tmp1/path.h");

	IS(opts.search_library("path.lib"), "path.lib");
	opts.add_library_path("tmp2");
	IS(opts.search_library("path.lib"), "tmp2/path.lib");

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
	opts.set_output(strout, strerr);	// redirect
	opts.cout() << "Test stdout" << std::endl;
	opts.cerr() << "Test stderr" << std::endl;

	IS(strout.str(), "Test stdout\n");
	IS(strerr.str(), "Test stderr\n");

	opts.set_output();					// back to default
	DIAG("Test stdout");
	opts.cout() << "Test stdout" << std::endl;
	DIAG("Test stderr");
	opts.cerr() << "Test stderr" << std::endl;
}

void test_options() {
	OK(opts.swap_ix_iy() == false);
	opts.set_swap_ix_iy(true);
	OK(opts.swap_ix_iy() == true);
}

int main() {
	START_TESTING();
	test_paths();
	test_redirect();
	test_options();
	DONE_TESTING();
}
