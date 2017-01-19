//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "token.h"
#include "parser.h"
#include "test.h"

#define T_TOKEN(id_, num_value_, str_value_) \
	IS(t->id(), id_); \
	IS(t->num_value(), num_value_); \
	IS(t->str_value(), str_value_)

int main()
{
	START_TESTING();
	Token* t;

	t = new Token;
	T_TOKEN(EOI, 0, "");
	delete t;

	t = new Token(12);
	T_TOKEN(TK_NUMBER, 12, "");
	delete t;

	t = new Token("L1");
	T_TOKEN(TK_STRING, 0, "L1");
	delete t;

	t = new Token;
	Token::free_token(t);

	DONE_TESTING();
}
