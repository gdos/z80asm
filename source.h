//-----------------------------------------------------------------------------
// z80asm source file
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#pragma once

#include <fstream>
#include <string>
#include <vector>

class SourceFile;

// input source line
struct SourceLine
{
	SourceLine(SourceFile *psource_, unsigned line_nr_, std::string text_)
		: psource(psource_), line_nr(line_nr_), text(text_) {}

	SourceFile	*psource;	// weak pointer
	unsigned	 line_nr;
	std::string	 text;		// "" if keep_lines = false
};

// input file
class SourceFile 
{
public:
	SourceFile(const std::string& filename_, bool keep_lines_ = false);		// keep previous lines?
	virtual ~SourceFile();

	const std::string& get_filename() const { return filename; }
	bool good() const { return ifs.good(); }

	// read new line, allocate internal SourceLine, return pointer to it; return NULL on end of file
	SourceLine *getline();

private:
	std::string		filename;
	std::ifstream	ifs;
	bool			keep_lines;			// if false(default), only last line is preserved
	std::vector<SourceLine *> lines;	// owns pointers
};

// stack of input files to read includes
class SourceStack
{
public:
	SourceStack(bool keep_lines_ = false);				// keep previous lines?
	virtual ~SourceStack();

	bool has_file(const std::string& filename);			// is the given file in the open stack?
	bool open(const std::string& filename);				// open a new file to read at this point

	SourceLine *getline();								// call getline from the top-of-stack, pop it when eof

private:
	bool			keep_lines;							// if false(default), only last line is preserved
	std::vector<SourceLine *> plines;					// points into SourceFile's SourceLines
	std::vector<SourceFile *> files;					// all files opened, owns the pointers
	std::vector<SourceFile *> pstack;					// current input stack, weak pointers
};

