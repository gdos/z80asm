//-----------------------------------------------------------------------------
// z80asm source file
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SOURCE_H_
#define SOURCE_H_

#include <fstream>
#include <string>
#include <vector>

class SourceFile;

// input source line
struct SourceLine {
public:
	SourceLine(SourceFile* weak_ptr_source_, int line_num_, std::string text_)
		: weak_ptr_source(weak_ptr_source_), line_num(line_num_), text(text_) {}

	SourceFile* weak_ptr_source;	
	int	        line_num;
	std::string	text;		// empty if keep_lines = false
};

// input file
class SourceFile {
public:
	SourceFile(const std::string& filename, bool keep_lines = false);		// keep previous lines?
	virtual ~SourceFile();

	const std::string& filename() const { return filename_; }
	bool good() const { return ifs_.good(); }

	// read new line, allocate internal SourceLine, return pointer to it; return NULL on end of file
	SourceLine* getline();

private:
	std::string		filename_;
	std::ifstream	ifs_;
	bool			keep_lines_;		// if false(default), only last line is preserved
	std::vector<SourceLine*> lines_;
};

// stack of input files to read includes
class SourceStack {
public:
	SourceStack(bool keep_lines = false);				// keep previous lines?
	virtual ~SourceStack();

	bool has_file(const std::string& filename);			// is the given file in the open stack?
	bool open(const std::string& filename);				// open a new file to read at this point

	SourceLine* getline();								// call getline from the top-of-stack, pop it when eof

private:
	bool    keep_lines_;					    		// if false(default), only last line is preserved
	std::vector<SourceLine*> weak_ptr_lines;			// points into SourceFile's SourceLines
	std::vector<SourceFile*> files;	    				// all files opened, owns the pointers
	std::vector<SourceFile*> weak_ptr_stack;			// current input stack
};

#endif // SOURCE_H_