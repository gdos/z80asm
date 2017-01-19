//-----------------------------------------------------------------------------
// z80asm source file
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SOURCE_H_
#define SOURCE_H_

#include "memcheck.h"
#include "fwd.h"
#include <fstream>
#include <string>
#include <vector>

class Source : noncopyable {
public:
	Source();
	virtual ~Source();

	bool has_file(const std::string& filename);		// is the given file in the open stack?
	bool open(const std::string& filename);			// open a new file to read at this point
	SrcLine* getline();								// read next line, NULL on end of input

private:
	std::vector<SrcFile*> files_;		// has all read files
	std::vector<SrcLine*> lines_;		// has all read lines
	std::vector<SrcFile*> stack_;		// weak pointer to open stack of files
};

class SrcFile : noncopyable {
public:
	SrcFile(const std::string& filename);
	~SrcFile();

	const std::string& filename() const { return filename_; }
	bool good() const { return ifs_.good(); }

	SrcLine* getline();		// read new allocated line; return NULL on end of file

private:
	std::string		filename_;
	std::ifstream	ifs_;
	int				line_nr_;
};

class SrcLine {
public:
	SrcLine(SrcFile* src_file, int line_nr, std::string text);	
	~SrcLine();

	SrcFile const* src_file() const { return src_file_; }
	int line_nr() const { return line_nr_; }
	const std::string& text() const { return text_; }
	void clear_text();

private:
	SrcFile*	src_file_;	// weak pointer
	int	        line_nr_;
	std::string	text_;		// kept during scanning and if opts.do_list() is true
};

#endif // SOURCE_H_
