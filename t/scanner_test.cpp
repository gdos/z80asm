//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "scanner.h"
#include "source.h"
#include "token.h"
#include "options.h"
#include "test.h"
#include <iostream>
#include <sstream>

#define T_TOKEN(id_, value_, text_) \
	IS(t->id(), id_); \
	IS(t->value(), value_); \
	IS(t->text(), text_)

const int HS_ONE =	IDENT_HASH('O', 'N', 'E', ' ', ' ');
const int HS_TWO =	IDENT_HASH('T', 'W', 'O', ' ', ' ');
const int HS_THREE =IDENT_HASH('T', 'H', 'R', 'E', 'E');
const int HS_FOUR = IDENT_HASH('F', 'O', 'U', 'R', ' ');

void test_before_init() {
	Scanner scan;
	Token* t;

	t = scan.peek();
	T_TOKEN(TK_EOI, 0, "");

	t = scan.peek(1);
	T_TOKEN(TK_EOI, 0, "");

	t = scan.peek(2);
	T_TOKEN(TK_EOI, 0, "");

	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");

	IS(scan.get_pos(), 0);
	scan.set_pos(0);
	IS(scan.get_pos(), 0);

	scan.flush();
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
}

void test_one_alnum_token() {
	Scanner scan;
	Token* t;

	SrcLine line(NULL, 0, "Ld");
	scan.init(&line);

	t = scan.next();
	T_TOKEN(TK_IDENT, KW_LD, "Ld");

	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, "Include");
	scan.init(&line);

	t = scan.next();
	T_TOKEN(TK_IDENT, KW_INCLUDE, "Include");

	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
}

void test_back_and_forward() {
	Scanner scan;
	Token* t;

	SrcLine line(NULL, 0, " one two three four ");
	scan.init(&line);

	t = scan.peek();
	T_TOKEN(TK_IDENT, HS_ONE, "one");

	t = scan.peek(1);
	T_TOKEN(TK_IDENT, HS_TWO, "two");

	t = scan.peek(2);
	T_TOKEN(TK_IDENT, HS_THREE, "three");

	t = scan.peek(3);
	T_TOKEN(TK_IDENT, HS_FOUR, "four");

	t = scan.peek(4);
	T_TOKEN(TK_EOI, 0, "");

	t = scan.peek(5);
	T_TOKEN(TK_EOI, 0, "");

	IS(scan.get_pos(), 0);
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_ONE, "one");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_TWO, "two");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_THREE, "three");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_FOUR, "four");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	scan.set_pos(2);

	t = scan.next();
	T_TOKEN(TK_IDENT, HS_THREE, "three");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_FOUR, "four");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
}

void test_comments_and_backslash() {
	Scanner scan;
	Token* t;

	SrcLine line(NULL, 0, "one ; um ; uno \ntwo \\three ; \\ four \nfour");
	scan.init(&line);

	t = scan.next();
	T_TOKEN(TK_IDENT, HS_ONE, "one");
	t = scan.next();
	T_TOKEN(TK_ENDL, 0, "");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_TWO, "two");
	t = scan.next();
	T_TOKEN(TK_ENDL, 0, "");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_THREE, "three");
	t = scan.next();
	T_TOKEN(TK_ENDL, 0, "");
	t = scan.next();
	T_TOKEN(TK_IDENT, HS_FOUR, "four");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
}

void test_errors() {
	Scanner scan;
	Token* t;
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "one\n`two");
		scan.init(&line);

		t = scan.next();
		T_TOKEN(TK_IDENT, HS_ONE, "one");

		t = scan.next();
		T_TOKEN(TK_ENDL, 0, "");

		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();

		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: syntax\n\tone\\`two\n\t    ^~~~\n", strerr.str());

		t = scan.next();
		T_TOKEN(TK_EOI, 0, "");
	}
	{
		std::ostringstream strout, strerr;
		Source file;

		create_test_file("test.asm", "one`two");
		OK(file.open("test.asm"));

		SrcLine* line = file.getline();
		OK(line != NULL);
		scan.init(line);

		t = scan.next();
		T_TOKEN(TK_IDENT, HS_ONE, "one");

		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();

		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("test.asm:1: Error: syntax\n\tone`two\n\t   ^~~~\n", strerr.str());

		t = scan.next();
		T_TOKEN(TK_EOI, 0, "");

		line = file.getline();
		OK(line == NULL);

		delete_test_file("test.asm");
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "'hello");
		scan.init(&line);
		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();
		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: missing closing quote\n\t'hello\n\t      ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "\"hello");
		scan.init(&line);
		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();
		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: missing closing quote\n\t\"hello\n\t      ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "'hello\n");
		scan.init(&line);
		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();
		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: missing closing quote\n\t'hello\\\n\t      ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "'he'\n");
		scan.init(&line);
		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();
		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: invalid single quoted character\n\t'he'\\\n\t    ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "''\n");
		scan.init(&line);
		opts.set_out_err(strout, strerr);
		t = scan.next();
		opts.set_out_err();
		T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: invalid single quoted character\n\t''\\\n\t  ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: expected file\n\tinclude\n\t       ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include\n");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: expected file\n\tinclude\\\n\t       ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include;xx");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: expected file\n\tinclude;xx\n\t       ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include'");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: missing closing quote\n\tinclude'\n\t        ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include\"");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: missing closing quote\n\tinclude\"\n\t        ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include<");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: missing closing bracket\n\tinclude<\n\t        ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include''");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: expected file\n\tinclude''\n\t         ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include\"\"");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: expected file\n\tinclude\"\"\n\t         ^~~~\n", strerr.str());
	}
	{
		std::ostringstream strout, strerr;

		SrcLine line(NULL, 0, "include<>");
		scan.init(&line);
		t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
		opts.set_out_err(strout, strerr);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		opts.set_out_err();
		t = scan.next(); T_TOKEN(TK_EOI, 0, "");

		T_STR("", strout.str());
		T_STR("0: Error: expected file\n\tinclude<>\n\t         ^~~~\n", strerr.str());
	}
}

void test_all_tokens() {
	Scanner scan;
	Token* t;

	SrcLine line(NULL, 0, " \x09\x0a\x0b\x0c\x0d!!=#%&&&()***+++,---./:<<< <=<>===>>> >=?[]^{}|||~one-INClude");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_ENDL, 0, "");
	t = scan.next(); T_TOKEN(TK_EXCLAM, 0, "");
	t = scan.next(); T_TOKEN(TK_NOT_EQUAL, 0, "");
	t = scan.next(); T_TOKEN(TK_HASH, 0, "");
	t = scan.next(); T_TOKEN(TK_PERCENT, 0, "");
	t = scan.next(); T_TOKEN(TK_AMPERSHAND2, 0, "");
	t = scan.next(); T_TOKEN(TK_AMPERSHAND, 0, "");
	t = scan.next(); T_TOKEN(TK_LPAREN, 0, "");
	t = scan.next(); T_TOKEN(TK_RPAREN, 0, "");
	t = scan.next(); T_TOKEN(TK_STAR2, 0, "");
	t = scan.next(); T_TOKEN(TK_STAR, 0, "");
	t = scan.next(); T_TOKEN(TK_PLUS2, 0, "");
	t = scan.next(); T_TOKEN(TK_PLUS, 0, "");
	t = scan.next(); T_TOKEN(TK_COMMA, 0, "");
	t = scan.next(); T_TOKEN(TK_MINUS2, 0, "");
	t = scan.next(); T_TOKEN(TK_MINUS, 0, "");
	t = scan.next(); T_TOKEN(TK_DOT, 0, "");
	t = scan.next(); T_TOKEN(TK_SLASH, 0, "");
	t = scan.next(); T_TOKEN(TK_COLON, 0, "");
	t = scan.next(); T_TOKEN(TK_LESS2, 0, "");
	t = scan.next(); T_TOKEN(TK_LESS, 0, "");
	t = scan.next(); T_TOKEN(TK_LESS_EQ, 0, "");
	t = scan.next(); T_TOKEN(TK_NOT_EQUAL, 0, "");
	t = scan.next(); T_TOKEN(TK_EQUAL2, 0, "");
	t = scan.next(); T_TOKEN(TK_EQUAL, 0, "");
	t = scan.next(); T_TOKEN(TK_GREATER2, 0, "");
	t = scan.next(); T_TOKEN(TK_GREATER, 0, "");
	t = scan.next(); T_TOKEN(TK_GREATER_EQ, 0, "");
	t = scan.next(); T_TOKEN(TK_QUESTION, 0, "");
	t = scan.next(); T_TOKEN(TK_LSQUARE, 0, "");
	t = scan.next(); T_TOKEN(TK_RSQUARE, 0, "");
	t = scan.next(); T_TOKEN(TK_CARET, 0, "");
	t = scan.next(); T_TOKEN(TK_LCURLY, 0, "");
	t = scan.next(); T_TOKEN(TK_RCURLY, 0, "");
	t = scan.next(); T_TOKEN(TK_VBAR2, 0, "");
	t = scan.next(); T_TOKEN(TK_VBAR, 0, "");
	t = scan.next(); T_TOKEN(TK_TILDE, 0, "");
	t = scan.next(); T_TOKEN(TK_IDENT, HS_ONE, "one");
	t = scan.next(); T_TOKEN(TK_MINUS, 0, "");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "INClude");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, "nz z nc c po pe p m");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_IDENT, KW_NZ, "nz");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_Z, "z");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_NC, "nc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_C, "c");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_PO, "po");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_PE, "pe");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_P, "p");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_M, "m");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, "b c d e h l a i r ixh ixl iyh iyl bc de hl af af'sp ix iy");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_IDENT, KW_B, "b");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_C, "c");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_D, "d");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_E, "e");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_H, "h");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_L, "l");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_A, "a");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_I, "i");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_R, "r");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IXH, "ixh");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IXL, "ixl");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IYH, "iyh");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IYL, "iyl");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_BC, "bc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DE, "de");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_HL, "hl");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_AF, "af");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_AF, "af'");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SP, "sp");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IX, "ix");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IY, "iy");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, 
		"adc add and bit call ccf cp cpd cpdr cpi cpir cpl daa dec di djnz ei ex exx halt "
		"im in inc ind indr ini inir jp jr ld ldd lddr ldi ldir neg nop or otdr otir out outd outi "
		"pop push res ret reti retn rl rla rlc rlca rld rr rra rrc rrca rrd rst sbc scf set sla sll sra srl sub xor"
	);
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_IDENT, KW_ADC, "adc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_ADD, "add");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_AND, "and");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_BIT, "bit");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CALL, "call");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CCF, "ccf");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CP, "cp");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CPD, "cpd");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CPDR, "cpdr");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CPI, "cpi");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CPIR, "cpir");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_CPL, "cpl");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DAA, "daa");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEC, "dec");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DI, "di");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DJNZ, "djnz");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_EI, "ei");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_EX, "ex");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_EXX, "exx");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_HALT , "halt");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IM, "im");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IN, "in");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_INC, "inc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IND, "ind");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_INDR, "indr");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_INI, "ini");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_INIR, "inir");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_JP, "jp");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_JR, "jr");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LD, "ld");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LDD, "ldd");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LDDR, "lddr");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LDI, "ldi");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LDIR, "ldir");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_NEG, "neg");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_NOP, "nop");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_OR, "or");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_OTDR, "otdr");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_OTIR, "otir");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_OUT, "out");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_OUTD, "outd");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_OUTI, "outi");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_POP, "pop");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_PUSH, "push");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RES, "res");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RET, "ret");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RETI, "reti");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RETN, "retn");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RL, "rl");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RLA, "rla");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RLC, "rlc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RLCA, "rlca");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RLD, "rld");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RR, "rr");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RRA, "rra");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RRC, "rrc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RRCA, "rrca");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RRD, "rrd");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_RST, "rst");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SBC, "sbc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SCF, "scf");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SET, "set");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SLA, "sla");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SLL, "sll");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SRA, "sra");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SRL, "srl");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SUB, "sub");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_XOR, "xor");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0,
		"binary defb defc define defm defq defs defw else endif extern global if ifdef ifndef "
		"line lston lstoff module org public section title undefine"
	);
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_IDENT, KW_BINARY, "binary");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFB, "defb");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFC, "defc");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFINE, "define");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFM, "defm");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFQ, "defq");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFS, "defs");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_DEFW, "defw");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_ELSE, "else");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_ENDIF, "endif");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_EXTERN, "extern");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_GLOBAL, "global");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IF, "if");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IFDEF, "ifdef");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_IFNDEF, "ifndef");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LINE, "line");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LSTON, "lston");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_LSTOFF, "lstoff");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_MODULE, "module");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_ORG, "org");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_PUBLIC, "public");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_SECTION, "section");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_TITLE, "title");
	t = scan.next(); T_TOKEN(TK_IDENT, KW_UNDEFINE, "undefine");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, "_Abc_123 Abc_123 123_Abc_0 0");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_IDENT, IDENT_HASH('_', 'A', 'B', 'C', '_'), "_Abc_123");
	t = scan.next(); T_TOKEN(TK_IDENT, IDENT_HASH('A', 'B', 'C', '_', '1'), "Abc_123");
	t = scan.next(); T_TOKEN(TK_NUMBER, 123, "");
	t = scan.next(); T_TOKEN(TK_IDENT, IDENT_HASH('_', 'A', 'B', 'C', '_'), "_Abc_0");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, "2147483647 2147483648 4294967295 0 1");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x80000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0xFFFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 1, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, 
		"   0000b     0011b      1111111111111111111111111111111b	    "
		"  @0000     @0011      @1111111111111111111111111111111		"
		"  %0000     %0011      %1111111111111111111111111111111		"
		" 0b0000    0b0011     0b1111111111111111111111111111111		");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000003, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000003, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000003, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000003, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0,
		"  0h  0ah 0FH  7FFFFFFFh	"
		" $0   $a  $F  $7FFFFFFF 	"
		"0x0  0xa 0xF 0x7FFFFFFF	"
		"0");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x0000000A, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x0000000F, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x0000000A, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x0000000F, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x00000000, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x0000000A, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x0000000F, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0x7FFFFFFF, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0,
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000"
		"1");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_NUMBER, 1, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0,
		"'a'"
		"'\\a'"
		"'\\b'"
		"'\\f'"
		"'\\n'"
		"'\\r'"
		"'\\t'"
		"'\\v'"
		"'\\\\'"
		"'\\''"
		"'\\\"'"
		"'\\?'"
		"'\\0'"
		"'\\377'"
		"'\\xff'");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_NUMBER, 'a', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\a', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\b', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\f', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\n', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\r', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\t', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\v', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\\', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '\'', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '"', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, '?', "");
	t = scan.next(); T_TOKEN(TK_NUMBER, 0, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, -1, "");
	t = scan.next(); T_TOKEN(TK_NUMBER, -1, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, "\"a\\a\\b\\f\\n\\r\\t\\v\\\\\\'\\\"\\?\\1\\377\\xff\"");
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_STRING, 0, "a\a\b\f\n\r\t\v\\'\"?\1\377\xff");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");

	line = SrcLine(NULL, 0, 
		"include file1;comment\n"
		"include file2\n"
		"include \"file3\"\n"
		"include 'file4'\n"
		"include <file5>\n"
	);
	scan.init(&line);
	t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
	OK(scan.scan_filename()); T_STR("file1", scan.text());
	t = scan.next(); T_TOKEN(TK_ENDL, 0, "");

	t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
	OK(scan.scan_filename()); T_STR("file2", scan.text());
	t = scan.next(); T_TOKEN(TK_ENDL, 0, "");

	t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
	OK(scan.scan_filename()); T_STR("file3", scan.text());
	t = scan.next(); T_TOKEN(TK_ENDL, 0, "");

	t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
	OK(scan.scan_filename()); T_STR("file4", scan.text());
	t = scan.next(); T_TOKEN(TK_ENDL, 0, "");

	t = scan.next(); T_TOKEN(TK_IDENT, KW_INCLUDE, "include");
	OK(scan.scan_filename()); T_STR("file5", scan.text());
	t = scan.next(); T_TOKEN(TK_ENDL, 0, "");

	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
	t = scan.next(); T_TOKEN(TK_EOI, 0, "");
}

int main()
{
	START_TESTING();

	test_before_init();
	test_one_alnum_token();
	test_back_and_forward();
	test_comments_and_backslash();
	test_errors();
	test_all_tokens();

	DONE_TESTING();
}
