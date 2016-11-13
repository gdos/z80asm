//-----------------------------------------------------------------------------
// z80asm parser
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#include "parser.h"
#include "gram.h"
#include "lemon.h"
#include <cstdlib>

// declarations of lemon functions

Parser::Parser(ObjectFile* obj)
	: obj_(obj) {
	parser_ = ParseAlloc(malloc);
}

Parser::~Parser() {
	ParseFree(parser_, free);
}

void Parser::parse(int yymajor, int yyminor) {
	Parse(parser_, yymajor, yyminor, this);
}

void Parser::end_parse() {
	Parse(parser_, 0, 0, this);
}
