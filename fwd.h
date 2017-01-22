//-----------------------------------------------------------------------------
// z80asm - forward declarations of interdependent classes
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef FWD_H_
#define FWD_H_

#include "memcheck.h"

class noncopyable {
public:
	noncopyable() {}
	virtual ~noncopyable() {}

private:
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&) {}
};

typedef unsigned char byte;

class Assembler;
class Expr;
class Module;
class Object;
class Opcode;
class Parser;
class PatchExpr;
class Scanner;
class Section;
class Source;
class SrcFile;
class SrcLine;
class Symtab;
class Token;

#endif // FWD_H_
