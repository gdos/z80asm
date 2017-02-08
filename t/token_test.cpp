//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "token.h"
#include "test.h"

#define T_TOKEN(id_, value_, text_) \
	IS(t->id(), id_); \
	IS(t->value(), value_); \
	IS(t->text(), text_)

int main()
{
	START_TESTING();
	Token* t;
	const char* hello = "hello";

	t = new Token;
	T_TOKEN(TK_EOI, 0, "");
	delete t;

	t = new Token(TK_ENDL);
	T_TOKEN(TK_ENDL, 0, "");
	delete t;

	t = new Token(TK_NUMBER, 23);
	T_TOKEN(TK_NUMBER, 23, "");
	delete t;

	t = new Token(TK_STRING, 0, hello, NULL);
	T_TOKEN(TK_STRING, 0, "");
	delete t;

	t = new Token(TK_STRING, 0, NULL, hello + 5);
	T_TOKEN(TK_STRING, 0, "");
	delete t;

	t = new Token(TK_STRING, 0, hello, hello + 5);
	T_TOKEN(TK_STRING, 0, "hello");
	delete t;

	t = new Token;
	Token::free_token(t);

	DONE_TESTING();
}
