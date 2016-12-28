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
		OK(util::getline(file, line).good()); IS(line, text)

#define T_EOF() \
		OK(!util::getline(file, line).good()); IS(line, ""); \
		OK(!util::getline(file, line).good()); IS(line, ""); \
		OK(!util::getline(file, line).good()); IS(line, "")

#define T_DELETE() \
	OK(file.eof()); \
	file.close(); \
	delete_test_file("test.1")

void test_getline()
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

void test_spew_slurp() {
	std::string text;

	delete_test_file("test.1");

	util::spew("test.1", "");
	IS(util::file_size("test.1"), 0);
	text = util::slurp("test.1");
	IS(text, "");

	util::spew("test.1", "1");
	IS(util::file_size("test.1"), 1);
	text = util::slurp("test.1");
	IS(text, "1\n");

	util::spew("test.1", "1\n");
	IS(util::file_size("test.1"), 2);
	text = util::slurp("test.1");
	IS(text, "1\n");

	util::spew("test.1", "1\n2");
	IS(util::file_size("test.1"), 3);
	text = util::slurp("test.1");
	IS(text, "1\n2\n");

	util::spew("test.1", "1\n2\n");
	IS(util::file_size("test.1"), 4);
	text = util::slurp("test.1");
	IS(text, "1\n2\n");

	delete_test_file("test.1");

	util::mkdir("test.1");
	OK(util::is_dir("test.1"));
	util::spew("test.1", "");	// expect perror
	util::rmdir("test.1");
	OK(!util::file_exists("test.1"));
}

void test_mk_rm() {
	std::string text;

	util::rmdir("test.1");
	OK(!util::file_exists("test.1"));

	util::mkdir("test.1");
	OK(util::is_dir("test.1"));

	util::mkdir("test.1");
	OK(util::is_dir("test.1"));

	create_test_file("test.1/hello.txt", "hello");
	OK(util::is_file("test.1/hello.txt"));
	text = util::slurp("test.1/hello.txt");
	IS(text, "hello\n");

	util::remove("test.1/hello.txt");
	OK(!util::file_exists("test.1/hello.txt"));

	util::remove("test.1/hello.txt");
	OK(!util::file_exists("test.1/hello.txt"));

	util::rmdir("test.1");
	OK(!util::file_exists("test.1"));

	util::rmdir("test.1");
	OK(!util::file_exists("test.1"));
}

void test_stat() {
	create_test_file("test.1", "hello");
	OK(util::file_exists("test.1"));
	OK(util::is_file("test.1"));
	OK(!util::is_dir("test.1"));
	IS(util::file_size("test.1"), 5);

	delete_test_file("test.1");
	OK(!util::file_exists("test.1"));
	OK(!util::is_file("test.1"));
	OK(!util::is_dir("test.1"));
	IS(util::file_size("test.1"), -1);	// expect perror

	util::mkdir("test.1");
	OK(util::file_exists("test.1"));
	OK(!util::is_file("test.1"));
	OK(util::is_dir("test.1"));
	IS(util::file_size("test.1"), -1);

	util::rmdir("test.1");
	OK(!util::file_exists("test.1"));
}

void test_search() {
	util::mkdir("test.1");
	util::mkdir("test.1/a");
	util::mkdir("test.1/b");

	delete_test_file("f1.asm");
	delete_test_file("f2.asm");

	create_test_file("test.1/a/f1.asm", "");

	create_test_file("test.1/b/f1.asm", "");
	create_test_file("test.1/b/f2.asm", "");

	std::vector<std::string> dirs;
	std::string file;

	file = util::file_search("f1.asm", dirs);
	IS(file, "f1.asm");	
	file = util::file_search("f2.asm", dirs);
	IS(file, "f2.asm");

	dirs.push_back("test.1/a");

	file = util::file_search("f1.asm", dirs);
	IS(file, "test.1/a/f1.asm");
	file = util::file_search("f2.asm", dirs);
	IS(file, "f2.asm");

	dirs.push_back("test.1/b");

	file = util::file_search("f1.asm", dirs);
	IS(file, "test.1/a/f1.asm");
	file = util::file_search("f2.asm", dirs);
	IS(file, "test.1/b/f2.asm");

	delete_test_file("test.1/a/f1.asm");
	delete_test_file("test.1/b/f1.asm");
	delete_test_file("test.1/b/f2.asm");
	util::rmdir("test.1/a");
	util::rmdir("test.1/b");
	util::rmdir("test.1");
	OK(!util::file_exists("test.1"));
}

#define T_PATH(file_, dirname_, filename_, basename_, extension_) \
	s = util::dirname(file_); IS(s, dirname_); \
	s = util::filename(file_); IS(s, filename_); \
	s = util::basename(file_); IS(s, basename_); \
	s = util::extension(file_); IS(s, extension_)

void test_path() {
	std::string s;

	T_PATH("file", ".", "file", "file", "");
	T_PATH("dir/file", "dir", "file", "file", "");
	T_PATH("dir\\file", "dir", "file", "file", "");
	T_PATH("dir//file", "dir", "file", "file", "");
	T_PATH("dir\\\\file", "dir", "file", "file", "");

	T_PATH("dir/", "dir", "", "", "");
	T_PATH("dir\\", "dir", "", "", "");
	T_PATH("dir//", "dir", "", "", "");
	T_PATH("dir\\\\", "dir", "", "", "");

	T_PATH(".", ".", ".", ".", "");
	T_PATH(".rc", ".", ".rc", ".rc", "");
	T_PATH("dir/.rc", "dir", ".rc", ".rc", "");
	T_PATH("dir.1/.rc", "dir.1", ".rc", ".rc", "");
	T_PATH("rc.x", ".", "rc.x", "rc", ".x");
	T_PATH("dir.1/rc.x", "dir.1", "rc.x", "rc", ".x");
}

int main()
{
	START_TESTING();
	test_getline();
	test_spew_slurp();
	test_mk_rm();
	test_stat();
	test_search();
	test_path();
	DONE_TESTING();
}
