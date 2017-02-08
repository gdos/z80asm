//-----------------------------------------------------------------------------
// z80asm scanner tokens
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef TOKEN_H_
#define TOKEN_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>

// token IDs share space with output from lemon (small numbers >= 1)
#include "gram.h"
enum {
	TK_EOI = 0,
	TK_EXCLAM = '!',
	TK_HASH = '#',
	TK_PERCENT = '%',
	TK_AMPERSHAND = '&',
	TK_LPAREN = '(',
	TK_RPAREN = ')',
	TK_STAR = '*',
	TK_PLUS = '+',
	TK_COMMA = ',',
	TK_MINUS = '-',
	TK_DOT = '.',
	TK_SLASH = '/',
	TK_COLON = ':',
	TK_LESS = '<',
	TK_EQUAL = '=',
	TK_GREATER = '>',
	TK_QUESTION = '?',
	TK_LSQUARE = '[',
	TK_RSQUARE = ']',
	TK_CARET = '^',
	TK_LCURLY = '{',
	TK_RCURLY = '}',
	TK_VBAR = '|',
	TK_TILDE = '~',

	TK_HASH2 = 128,
	TK_AMPERSHAND2,
	TK_STAR2,
	TK_PLUS2,
	TK_MINUS2,
	TK_LESS2,
	TK_LESS_EQ,
	TK_NOT_EQUAL,
	TK_GREATER2,
	TK_GREATER_EQ,
	TK_VBAR2,
	TK_EQUAL2,

	TK_NUMBER,
	TK_STRING,

	// reserved words
	TK_NZ, 
	TK_Z, 
	TK_NC, 
	TK_C, 
	TK_PO, 
	TK_PE, 
	TK_P, 
	TK_M,
	
	TK_B,
	TK_D,
	TK_E,
	TK_H,
	TK_L,
	TK_A,
	TK_F,
	TK_I,
	TK_R,
	TK_IXH,
	TK_IXL,
	TK_IYH,
	TK_IYL,

	TK_BC,
	TK_DE,
	TK_HL,
	TK_AF,
	TK_SP,
	TK_IX,
	TK_IY,

	TK_ASMPC,

	// words that can be used as label names or opcodes; scanner returns Token(TK_IDENT, TK_ADD) for "add"
	TK_IDENT,

	TK_ADC,
	TK_ADD,
	TK_AND,
	TK_BIT,
	TK_CALL,
	TK_CCF,
	TK_CP,
	TK_CPD,
	TK_CPDR,
	TK_CPI,
	TK_CPIR,
	TK_CPL,
	TK_DAA,
	TK_DEC,
	TK_DI,
	TK_DJNZ,
	TK_EI,
	TK_EX,
	TK_EXX,
	TK_HALT,
	TK_IM,
	TK_IN,
	TK_IN0,
	TK_INC,
	TK_IND,
	TK_INDR,
	TK_INI,
	TK_INIR,
	TK_JP,
	TK_JR,
	TK_LD,
	TK_LDD,
	TK_LDDR,
	TK_LDI,
	TK_LDIR,
	TK_MLT,
	TK_NEG,
	TK_NOP,
	TK_OR,
	TK_OTDM,
	TK_OTDMR,
	TK_OTDR,
	TK_OTIM,
	TK_OTIMR,
	TK_OTIR,
	TK_OUT,
	TK_OUT0,
	TK_OUTD,
	TK_OUTI,
	TK_POP,
	TK_PUSH,
	TK_RES,
	TK_RET,
	TK_RETI,
	TK_RETN,
	TK_RL,
	TK_RLA,
	TK_RLC,
	TK_RLCA,
	TK_RLD,
	TK_RR,
	TK_RRA,
	TK_RRC,
	TK_RRCA,
	TK_RRD,
	TK_RST,
	TK_SBC,
	TK_SCF,
	TK_SET,
	TK_SLA,
	TK_SLL,
	TK_SLP,
	TK_SRA,
	TK_SRL,
	TK_SUB,
	TK_TST,
	TK_TSTIO,
	TK_XOR,

	TK_BINARY,
	TK_DEFB,
	TK_DEFC,
	TK_DEFINE,
	TK_DEFM,
	TK_DEFQ,
	TK_DEFS,
	TK_DEFW,
	TK_ELSE,
	TK_END,
	TK_ENDIF,
	TK_EXTERN,
	TK_GLOBAL,
	TK_IF,
	TK_IFDEF,
	TK_IFNDEF,
	TK_INCLUDE,
	TK_LINE,
	TK_LSTON,
	TK_LSTOFF,
	TK_MODULE,
	TK_ORG,
	TK_PUBLIC,
	TK_SECTION,
	TK_TITLE,
	TK_UNDEFINE,
};

class Token {
public:
	Token(int id = TK_EOI, int value = 0, const char* ts = NULL, const char* te = NULL);
	virtual ~Token();

	int id() const { return id_; }
	int value() const { return value_; }
	const char* ts() const { return ts_; }
	const char* te() const { return te_; }
	std::string text() const { return (ts_ != NULL && te_ != NULL) ? std::string(ts_, te_) : ""; }

	static void free_token(Token* token);		// needed by lemon

private:
	int			id_;
	int			value_;
	const char *ts_, *te_;	// start and end of token
};

#endif // TOKEN_H_
