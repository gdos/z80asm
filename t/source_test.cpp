//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "source.h"
#include "test.h"

#define T_CREATE(data_) \
		if (file) { delete file; file = NULL; } \
		create_test_file("test.1", data_); \
		file = new SrcFile("test.1"); \
        IS(file->filename(), "test.1"); \
		OK(file->good())

#define T_READ(line_nr_, text_) \
		line = file->getline(); \
		OK(line != NULL); \
		OK(line->src_file() == file); \
		IS(line->line_nr(), line_nr_); \
		IS(line->text(), text_); \
		line->clear_text();	\
		IS(line->text(), ""); \
		delete line

#define T_EOF() \
		line = file->getline(); OK(line == NULL); \
		line = file->getline(); OK(line == NULL); \
		line = file->getline(); OK(line == NULL)

#define T_DELETE() \
	delete file; file = NULL; \
	delete_test_file("test.1")


int main() {
	START_TESTING();

	SrcFile* file = NULL;
	SrcLine* line;

	// non-existent file
	delete_test_file("test.1");
	file = new SrcFile("test.1");	// perror
	IS(file->filename(), "test.1");
	OK(!file->good());
	T_EOF();
	T_DELETE();

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

	T_READ(1, "f1.asm");
	T_READ(2, "f2.asm");
	T_READ(3, "f3.asm");
	T_READ(4, "f4.asm");
	T_READ(5, "");
	T_READ(6, "f5.asm");
	T_EOF();
	T_DELETE();

	// end file in all new-line types, plus invalid LF-CR
	T_CREATE("f1.asm"); T_READ(1, "f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\n"); T_READ(1, "f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\r"); T_READ(1, "f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\r\n"); T_READ(1, "f1.asm"); T_EOF(); T_DELETE();
	T_CREATE("f1.asm\n\r"); T_READ(1, "f1.asm"); T_READ(2, ""); T_EOF(); T_DELETE();

	// Source stack
	create_test_file("test.1", "11\n12\n13");
	create_test_file("test.2", "21\n22\n23");
	delete_test_file("test.3");

	for (int i = 0; i < 2; ++i) {
		bool keep = (i == 1);

		Source st(keep);
		SrcLine* line;
		SrcLine* lines[6];

		OK(!st.has_file("test.1")); OK(!st.has_file("test.2")); OK(!st.has_file("test.3")); 

		OK(st.open("test.1"));
		OK(st.has_file("test.1")); OK(!st.has_file("test.2")); OK(!st.has_file("test.3")); 

		OK(!st.open("test.3"));		// perror
		OK(st.has_file("test.1")); OK(!st.has_file("test.2")); OK(!st.has_file("test.3")); 

		lines[0] = line = st.getline(); 
		IS(line->src_file()->filename(), "test.1"); IS(line->line_nr(), 1); IS(line->text(), "11");

		OK(st.open("test.2"));
		OK(st.has_file("test.1")); OK(st.has_file("test.2")); OK(!st.has_file("test.3")); 

		lines[1] = line = st.getline(); 
		IS(line->src_file()->filename(), "test.2"); IS(line->line_nr(), 1); IS(line->text(), "21");

		lines[2] = line = st.getline(); 
		IS(line->src_file()->filename(), "test.2"); IS(line->line_nr(), 2); IS(line->text(), "22");

		lines[3] = line = st.getline();
		IS(line->src_file()->filename(), "test.2"); IS(line->line_nr(), 3); IS(line->text(), "23");

		lines[4] = line = st.getline();
		IS(line->src_file()->filename(), "test.1"); IS(line->line_nr(), 2); IS(line->text(), "12");

		lines[5] = line = st.getline(); 
		IS(line->src_file()->filename(), "test.1"); IS(line->line_nr(), 3); IS(line->text(), "13");

		line = st.getline(); 
		OK(line == NULL); 
		
		line = st.getline();
		OK(line == NULL);

		// check keep_lines
		line = lines[0];
		IS(line->src_file()->filename(), "test.1"); IS(line->line_nr(), 1); IS(line->text(), keep ? "11" : "");

		line = lines[1];
		IS(line->src_file()->filename(), "test.2"); IS(line->line_nr(), 1); IS(line->text(), keep ? "21" : "");

		line = lines[2];
		IS(line->src_file()->filename(), "test.2"); IS(line->line_nr(), 2); IS(line->text(), keep ? "22" : "");

		line = lines[3];
		IS(line->src_file()->filename(), "test.2"); IS(line->line_nr(), 3); IS(line->text(), keep ? "23" : "");

		line = lines[4];
		IS(line->src_file()->filename(), "test.1"); IS(line->line_nr(), 2); IS(line->text(), keep ? "12" : "");

		line = lines[5];
		IS(line->src_file()->filename(), "test.1"); IS(line->line_nr(), 3); IS(line->text(), keep ? "13" : "");
	}

	delete_test_file("test.1");
	delete_test_file("test.2");
	delete_test_file("test.3");

	DONE_TESTING();
}
