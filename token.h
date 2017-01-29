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

// compact alnum character in 6 bits for hashing - as macro to get compile-time constants
#define ALNUM_HASH(n)	((n) >= 'a' ?		(n) - 'a' + 10 + 1 : \
						 (n) == '_' ?		            10 + 1 + 26 : \
						 (n) >= 'A' ?		(n) - 'A' + 10 + 1 : \
						 (n) >= '0' ?       (n) - '0' + 1 : \
											0)
#define IDENT_HASH(a,b,c,d,e) \
						((ALNUM_HASH(a) << 0) + \
						 (ALNUM_HASH(b) << 6) + \
						 (ALNUM_HASH(c) << 12) + \
						 (ALNUM_HASH(d) << 18) + \
						 (ALNUM_HASH(e) << 24))

// hash values of keywords
#define KW_NZ	IDENT_HASH('N','Z',' ',' ',' ')
#define KW_Z	IDENT_HASH('Z',' ',' ',' ',' ')
#define KW_NC	IDENT_HASH('N','C',' ',' ',' ')
#define KW_C	IDENT_HASH('C',' ',' ',' ',' ')
#define KW_PO	IDENT_HASH('P','O',' ',' ',' ')
#define KW_PE	IDENT_HASH('P','E',' ',' ',' ')
#define KW_P	IDENT_HASH('P',' ',' ',' ',' ')
#define KW_M	IDENT_HASH('M',' ',' ',' ',' ')

#define KW_B	IDENT_HASH('B',' ',' ',' ',' ')
#define KW_D	IDENT_HASH('D',' ',' ',' ',' ')
#define KW_E	IDENT_HASH('E',' ',' ',' ',' ')
#define KW_H	IDENT_HASH('H',' ',' ',' ',' ')
#define KW_L	IDENT_HASH('L',' ',' ',' ',' ')
#define KW_A	IDENT_HASH('A',' ',' ',' ',' ')
#define KW_F	IDENT_HASH('F',' ',' ',' ',' ')
#define KW_I	IDENT_HASH('I',' ',' ',' ',' ')
#define KW_R	IDENT_HASH('R',' ',' ',' ',' ')
#define KW_IXH	IDENT_HASH('I','X','H',' ',' ')
#define KW_IXL	IDENT_HASH('I','X','L',' ',' ')
#define KW_IYH	IDENT_HASH('I','Y','H',' ',' ')
#define KW_IYL	IDENT_HASH('I','Y','L',' ',' ')

#define KW_BC	IDENT_HASH('B','C',' ',' ',' ')
#define KW_DE	IDENT_HASH('D','E',' ',' ',' ')
#define KW_HL	IDENT_HASH('H','L',' ',' ',' ')
#define KW_AF	IDENT_HASH('A','F',' ',' ',' ')
#define KW_SP	IDENT_HASH('S','P',' ',' ',' ')
#define KW_IX	IDENT_HASH('I','X',' ',' ',' ')
#define KW_IY	IDENT_HASH('I','Y',' ',' ',' ')

#define KW_ADC	IDENT_HASH('A','D','C',' ',' ')
#define KW_ADD	IDENT_HASH('A','D','D',' ',' ')
#define KW_AND	IDENT_HASH('A','N','D',' ',' ')
#define KW_BIT	IDENT_HASH('B','I','T',' ',' ')
#define KW_CALL	IDENT_HASH('C','A','L','L',' ')
#define KW_CCF	IDENT_HASH('C','C','F',' ',' ')
#define KW_CP	IDENT_HASH('C','P',' ',' ',' ')
#define KW_CPD	IDENT_HASH('C','P','D',' ',' ')
#define KW_CPDR	IDENT_HASH('C','P','D','R',' ')
#define KW_CPI	IDENT_HASH('C','P','I',' ',' ')
#define KW_CPIR	IDENT_HASH('C','P','I','R',' ')
#define KW_CPL	IDENT_HASH('C','P','L',' ',' ')
#define KW_DAA	IDENT_HASH('D','A','A',' ',' ')
#define KW_DEC	IDENT_HASH('D','E','C',' ',' ')
#define KW_DI	IDENT_HASH('D','I',' ',' ',' ')
#define KW_DJNZ	IDENT_HASH('D','J','N','Z',' ')
#define KW_EI	IDENT_HASH('E','I',' ',' ',' ')
#define KW_EX	IDENT_HASH('E','X',' ',' ',' ')
#define KW_EXX	IDENT_HASH('E','X','X',' ',' ')
#define KW_HALT	IDENT_HASH('H','A','L','T',' ')
#define KW_IM	IDENT_HASH('I','M',' ',' ',' ')
#define KW_IN	IDENT_HASH('I','N',' ',' ',' ')
#define KW_INC	IDENT_HASH('I','N','C',' ',' ')
#define KW_IND	IDENT_HASH('I','N','D',' ',' ')
#define KW_INDR	IDENT_HASH('I','N','D','R',' ')
#define KW_INI	IDENT_HASH('I','N','I',' ',' ')
#define KW_INIR	IDENT_HASH('I','N','I','R',' ')
#define KW_JP	IDENT_HASH('J','P',' ',' ',' ')
#define KW_JR	IDENT_HASH('J','R',' ',' ',' ')
#define KW_LD	IDENT_HASH('L','D',' ',' ',' ')
#define KW_LDD	IDENT_HASH('L','D','D',' ',' ')
#define KW_LDDR	IDENT_HASH('L','D','D','R',' ')
#define KW_LDI	IDENT_HASH('L','D','I',' ',' ')
#define KW_LDIR	IDENT_HASH('L','D','I','R',' ')
#define KW_NEG	IDENT_HASH('N','E','G',' ',' ')
#define KW_NOP	IDENT_HASH('N','O','P',' ',' ')
#define KW_OR	IDENT_HASH('O','R',' ',' ',' ')
#define KW_OTDR	IDENT_HASH('O','T','D','R',' ')
#define KW_OTIR	IDENT_HASH('O','T','I','R',' ')
#define KW_OUT	IDENT_HASH('O','U','T',' ',' ')
#define KW_OUTD	IDENT_HASH('O','U','T','D',' ')
#define KW_OUTI	IDENT_HASH('O','U','T','I',' ')
#define KW_POP	IDENT_HASH('P','O','P',' ',' ')
#define KW_PUSH	IDENT_HASH('P','U','S','H',' ')
#define KW_RES	IDENT_HASH('R','E','S',' ',' ')
#define KW_RET	IDENT_HASH('R','E','T',' ',' ')
#define KW_RETI	IDENT_HASH('R','E','T','I',' ')
#define KW_RETN	IDENT_HASH('R','E','T','N',' ')
#define KW_RL	IDENT_HASH('R','L',' ',' ',' ')
#define KW_RLA	IDENT_HASH('R','L','A',' ',' ')
#define KW_RLC	IDENT_HASH('R','L','C',' ',' ')
#define KW_RLCA	IDENT_HASH('R','L','C','A',' ')
#define KW_RLD	IDENT_HASH('R','L','D',' ',' ')
#define KW_RR	IDENT_HASH('R','R',' ',' ',' ')
#define KW_RRA	IDENT_HASH('R','R','A',' ',' ')
#define KW_RRC	IDENT_HASH('R','R','C',' ',' ')
#define KW_RRCA	IDENT_HASH('R','R','C','A',' ')
#define KW_RRD	IDENT_HASH('R','R','D',' ',' ')
#define KW_RST	IDENT_HASH('R','S','T',' ',' ')
#define KW_SBC	IDENT_HASH('S','B','C',' ',' ')
#define KW_SCF	IDENT_HASH('S','C','F',' ',' ')
#define KW_SET	IDENT_HASH('S','E','T',' ',' ')
#define KW_SLA	IDENT_HASH('S','L','A',' ',' ')
#define KW_SLL	IDENT_HASH('S','L','L',' ',' ')
#define KW_SRA	IDENT_HASH('S','R','A',' ',' ')
#define KW_SRL	IDENT_HASH('S','R','L',' ',' ')
#define KW_SUB	IDENT_HASH('S','U','B',' ',' ')
#define KW_XOR	IDENT_HASH('X','O','R',' ',' ')

#define KW_BINARY	IDENT_HASH('B','I','N','A','R')
#define KW_DEFB		IDENT_HASH('D','E','F','B',' ')
#define KW_DEFC		IDENT_HASH('D','E','F','C',' ')
#define KW_DEFINE	IDENT_HASH('D','E','F','I','N')
#define KW_DEFM		IDENT_HASH('D','E','F','M',' ')
#define KW_DEFQ		IDENT_HASH('D','E','F','Q',' ')
#define KW_DEFS		IDENT_HASH('D','E','F','S',' ')
#define KW_DEFW		IDENT_HASH('D','E','F','W',' ')
#define KW_ELSE		IDENT_HASH('E','L','S','E',' ')
#define KW_ENDIF	IDENT_HASH('E','N','D','I','F')
#define KW_EXTERN	IDENT_HASH('E','X','T','E','R')
#define KW_GLOBAL	IDENT_HASH('G','L','O','B','A')
#define KW_IF		IDENT_HASH('I','F',' ',' ',' ')
#define KW_IFDEF	IDENT_HASH('I','F','D','E','F')
#define KW_IFNDEF	IDENT_HASH('I','F','N','D','E')
#define KW_INCLUDE	IDENT_HASH('I','N','C','L','U')
#define KW_LINE		IDENT_HASH('L','I','N','E',' ')
#define KW_LSTON	IDENT_HASH('L','S','T','O','N')
#define KW_LSTOFF	IDENT_HASH('L','S','T','O','F')
#define KW_MODULE	IDENT_HASH('M','O','D','U','L')
#define KW_ORG		IDENT_HASH('O','R','G',' ',' ')
#define KW_PUBLIC	IDENT_HASH('P','U','B','L','I')
#define KW_SECTION	IDENT_HASH('S','E','C','T','I')
#define KW_TITLE	IDENT_HASH('T','I','T','L','E')
#define KW_UNDEFINE	IDENT_HASH('U','N','D','E','F')

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

	TK_IDENT = 256,

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

	TK_INCLUDE,
	TK_OPCODE_VOID,
	TK_STRING
};

class Token {
public:
	Token(int id = TK_EOI, int value = 0, const char* ts = NULL, const char* te = NULL);
	virtual ~Token();

	int id() const { return id_; }
	int value() const { return value_; }
	const char* ts() const { return ts_; }
	const char* te() const { return te_; }
	std::string text() const { return ts_ ? std::string(ts_, te_) : ""; }

	static void free_token(Token* token);		// needed by lemon

private:
	int			id_;
	int			value_;
	const char *ts_, *te_;	// start and end of token
};

#endif // TOKEN_H_
