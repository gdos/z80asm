//-----------------------------------------------------------------------------
// z80asm lemon functions declarations
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef LEMON_H_
#define LEMON_H_

#include "memcheck.h"
#include <cstdlib>

class Parser;

extern void *ParseAlloc(void *(*mallocProc)(size_t));
extern void ParseFree(void *yyp, void(*freeProc)(void*));
extern void Parse(void *yyp, int yymajor, int yyminor, Parser* parser);

#endif // ndef LEMON_H_
