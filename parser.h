//-----------------------------------------------------------------------------
// z80asm parser
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef PARSER_H_
#define PARSER_H_

#include "memcheck.h"
#include "fwd.h"
#include "lemon.h"

class Parser : noncopyable {
public:
	Parser(Object* object);
	virtual ~Parser();

	Object* object() { return object_; }
	SrcLine* line() { return line_; }

	bool parse();

protected:
	Object*		object_;		// weak pointer
	SrcLine*	line_;			// weak pointer
	void*		lemon_;

	bool parse_statement(Scanner* scan);
	bool parse_include(Scanner* scan);
	bool parse_opcode_void(Scanner* scan);
};

#endif // ndef PARSER_H_
