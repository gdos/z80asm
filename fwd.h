//-----------------------------------------------------------------------------
// z80asm - forward declarations of interdependent classes
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef FWD_H_
#define FWD_H_

class Env;
class Expr;
class Instr;
class Module;
class ObjectFile;
class Opcode;
class Scanner;
class Section;
class SourceFile;
class SourceStack;
class Symbol;
class SymbolTable;
struct SrcFile;
struct SrcLine;
struct SrcPos;

class noncopyable {
public:
	noncopyable() {}
	virtual ~noncopyable() {}

private:
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&);
};

#endif // FWD_H_
