//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "util.h"
#include "test.h"

#define T_CREATE(data) \
		create_test_file("test.1", data); \
		file.open("test.1", std::ios::in | std::ios::binary); \
		OK(file.good())

#define T_READ(text) \
		OK(safe_getline(file, line).good()); IS(line, text)

#define T_EOF() \
		OK(!safe_getline(file, line).good()); IS(line, ""); \
		OK(!safe_getline(file, line).good()); IS(line, ""); \
		OK(!safe_getline(file, line).good()); IS(line, "")

#define T_DELETE() \
	OK(file.eof()); \
	file.close(); \
	delete_test_file("test.1")

void test_safe_getline()
{
	std::string line;
	std::ifstream file;

	// empty file
	T_CREATE("");
	T_EOF();
	T_DELETE();

	// file with all new-line types, plus invalid LF-CR
	T_CREATE("f1.asm\n"
		"f2.asm\r"
		"f3.asm\r\n"
		"f4.asm\n\r"
		"f5.asm");

	T_READ("f1.asm");
	T_READ("f2.asm");
	T_READ("f3.asm");
	T_READ("f4.asm");
	T_READ("");
	T_READ("f5.asm");
	T_EOF();
	T_DELETE();

	// end file in all new-line types, plus invalid LF-CR
	T_CREATE("f1.asm"); T_READ("f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\n"); T_READ("f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\r"); T_READ("f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\r\n"); T_READ("f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\n\r"); T_READ("f1.asm"); T_READ(""); T_EOF(); T_DELETE();
}

int main()
{
	START_TESTING();
	test_safe_getline();
	DONE_TESTING();
}
