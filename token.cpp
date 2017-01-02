//-----------------------------------------------------------------------------
// z80asm scanner tokens
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "token.h"
#include "lemon.h"
#include "gram.h"

Token::Token()
	: id_(TK_END_INPUT), num_value_(0), str_value_("") {}

Token::Token(int num_value)
	: id_(TK_NUMBER), num_value_(num_value), str_value_("") {}

Token::Token(const std::string& str_value)
	: id_(TK_STRING), num_value_(0), str_value_(str_value) {}

Token::~Token() {}

void Token::free_token(Token* token) {
	delete token;
}
