//-----------------------------------------------------------------------------
// z80asm Lemon grammar
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

%include {
#include "parser.h"
#include "options.h"
#include "message.h"
#include "token.h"
#include <iostream>
}  
   
%token_type 	{Token*}
%default_type	{Token*}
%token_destructor {Token::free_token($$);}

%token_prefix	TK_
%extra_argument	{Parser* parser}
%syntax_error 	{err::syntax(parser->line());}
%parse_failure 	{err::failure(parser->line());}

file 		::= statements.
statements 	::= statements statement.

statement	::= ENDL.
statement	::= NOP ENDL.
statement	::= ORG NUMBER.
