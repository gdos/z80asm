//-----------------------------------------------------------------------------
// z80asm scanner
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SCANNER_H_
#define SCANNER_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>

class Scanner : noncopyable {
public:
	Scanner();
	virtual ~Scanner();

	void init(SrcLine* line);	// prepare to scan line
	int get_opcode();			// return next opcode or label
	bool get_filename();		// return file name of INCLUDE in text_, syntax error otherwise
	bool get_end_statement();	// check for end of statement, syntax error otherwise

	SrcLine* from() { return line_; }
	int column() { return line_ ? p_ - ts_ + 1 : 0; }
	const std::string& text() const { return text_; }
	int number() const { return number_; }
	Opcode* opcode() { return opcode_; }

	void flush();				// flush input
	
								// show errors and flush input; errfunc() is called for error message
	void error(void(*errfunc)(SrcLine*, int));

private:
	SrcLine*	line_;			// weak pointer to line being parsed
	std::string	text_;			// token text value
	int			number_;		// token number value
	Opcode*		opcode_;		// weak pointer to opcode

	const char*	ts_;			// scan pointers
	const char* p_;
	const char* marker_;
	const char* ctxmarker_;
};

#endif // SCANNER_H_
