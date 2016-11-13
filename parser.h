//-----------------------------------------------------------------------------
// z80asm parser
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef PARSER_H_
#define PARSER_H_

#include "fwd.h"

class Parser {
public:
    Parser(ObjectFile* obj);
    virtual ~Parser();

    void parse(int yymajor, int yyminor);
	void end_parse();

private:
    ObjectFile*     obj_;
	void*			parser_;		// lemon parser
};

#endif // PARSER_H_
