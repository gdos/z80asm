//-----------------------------------------------------------------------------
// z80asm scanner tokens
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "token.h"
#include "scanner.h"

Token::Token(int id, int value, const char* ts, const char* te)
	: id_(id), value_(value), ts_(ts), te_(te) {}

Token::~Token() {}

void Token::free_token(Token* token) {
	delete token;
}
