//-----------------------------------------------------------------------------
// z80asm scanner
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SCANNER_H_
#define SCANNER_H_

#include "memcheck.h"
#include "fwd.h"
#include "token.h"
#include <string>
#include <vector>

class Scanner : noncopyable {
public:
	Scanner();
	virtual ~Scanner();

	void init(SrcLine* line);		// prepare to scan line
	Token* peek(int n = 0);			// peek Nth token
	Token* next();					// return peek(0), advance pointer
	int get_pos() const;			// return current input pos
	void set_pos(int pos);			// revert to point of get_pos()

	void skip_space();				// advance past blanks
	bool eoi();						// check for TK_EOI
	bool scan_filename();			// scan argument to INCLUDE and BINARY - enclosed in '', "" or <>, no C-escapes
	bool scan_eos();				// get end of statment, error otherwise

	SrcLine* line() { return line_; }
	const std::string& text() const { return text_; }

	int Scanner::column();
	void error(void(*errfunc)(SrcLine*, int));
	void warning(void(*errfunc)(SrcLine*, int));
	void flush();					// flush input

private:
	SrcLine*	line_;				// weak pointer to line being parsed
	const char* p_;					// scan pointers
	const char* marker_;
	const char* ctxmarker_;
	std::vector<Token>	tokens_;	// list of tokens from start of line
	unsigned			tok_p_;		// current token
	std::string			text_;		// last TK_STRING token

	static Token eoi_token;
	static SrcLine default_line;

	bool push_next();				// push next token from line to tokens_, return false at end of input
	int number(const char* ts, int base);	// convert number at ts
};

#endif // SCANNER_H_
