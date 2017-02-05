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

Scanner scan;
Token* t;
SrcLine line;

#define T_INIT(input_) \
		line = SrcLine(NULL, 0, input_); \
		scan.init(&line)

#define T_TOKEN(id_, value_, text_) \
		IS(t->id(), id_); \
		IS(t->value(), value_); \
		IS(t->text(), text_)

#define T_NEXT3(id_, value_, text_) \
		t = scan.next(); T_TOKEN(id_, value_, text_)

#define T_NEXT2(id_, value_) \
		T_NEXT3(id_, value_, "")

#define T_NEXT(id_) \
		T_NEXT2(id_, 0)

#define T_END() \
		t = scan.next(); T_TOKEN(TK_EOI, 0, ""); \
		t = scan.next(); T_TOKEN(TK_EOI, 0, "")

#define T_SCAN3(input_, id_, value_, text_) \
		T_INIT(input_); \
		T_NEXT3(id_, value_, text_); \
		T_END()

#define T_SCAN2(input_, id_, value_) \
		T_SCAN3(input_, id_, value_, "")

#define T_SCAN(input_, id_) \
		T_SCAN2(input_, id_, 0)

void test_before_init() {
	
	T_INIT("");

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

void test_back_and_forward() {
	line = SrcLine(NULL, 0, " one two three four ");
	scan.init(&line);

	t = scan.peek();
	T_TOKEN(TK_IDENT, 0, "one");

	t = scan.peek(1);
	T_TOKEN(TK_IDENT, 0, "two");

	t = scan.peek(2);
	T_TOKEN(TK_IDENT, 0, "three");

	t = scan.peek(3);
	T_TOKEN(TK_IDENT, 0, "four");

	t = scan.peek(4);
	T_TOKEN(TK_EOI, 0, "");

	t = scan.peek(5);
	T_TOKEN(TK_EOI, 0, "");

	IS(scan.get_pos(), 0);
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "one");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "two");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "three");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "four");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	scan.set_pos(2);

	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "three");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "four");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
}

void test_comments_and_backslash() {
	line = SrcLine(NULL, 0, "one ; um ; uno \ntwo \\three ; \\ four \nfour");
	scan.init(&line);

	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "one");
	t = scan.next();
	T_TOKEN(TK_ENDL, 0, "");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "two");
	t = scan.next();
	T_TOKEN(TK_ENDL, 0, "");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "three");
	t = scan.next();
	T_TOKEN(TK_ENDL, 0, "");
	t = scan.next();
	T_TOKEN(TK_IDENT, 0, "four");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
	t = scan.next();
	T_TOKEN(TK_EOI, 0, "");
}

void test_errors() {
	{
		START_CAPTURE();
		T_INIT("one\n`two");
		T_NEXT3(TK_IDENT, 0, "one");
		T_NEXT(TK_ENDL);
		T_END();
		END_CAPTURE(
			"",
			"0: Error: syntax\n"
			"\tone\\`two\n"
			"\t    ^~~~\n");
	}
	{
		START_CAPTURE();
		
		Source file;

		create_test_file("test.asm", "one`two");
		OK(file.open("test.asm"));

		SrcLine* line = file.getline();
		OK(line != NULL);
		scan.init(line);

		T_NEXT3(TK_IDENT, 0, "one");
		T_END();

		line = file.getline();
		OK(line == NULL);

		delete_test_file("test.asm");

		END_CAPTURE(
			"", 
			"test.asm:1: Error: syntax\n" 
			"\tone`two\n"
			"\t   ^~~~\n");
	}
}

void test_tokens() {
	T_INIT(" \x09" "!" "\x0a" "$ ASMPC ASMPC1" "\x0b\x0c\x0d" ".");
	T_NEXT(TK_EXCLAM);
	T_NEXT(TK_ENDL);
	T_NEXT(TK_ASMPC);
	T_NEXT(TK_ASMPC);
	T_NEXT3(TK_IDENT, 0, "ASMPC1");
	T_NEXT(TK_DOT);
	T_END();

	T_INIT("!!=");
	T_NEXT(TK_EXCLAM);
	T_NEXT(TK_NOT_EQUAL);
	T_END();

	T_INIT("###f#");
	T_NEXT(TK_HASH2);
	T_NEXT2(TK_NUMBER, 15);
	T_NEXT(TK_HASH);
	T_END();

	T_INIT("$$f");
	T_NEXT(TK_ASMPC);
	T_NEXT2(TK_NUMBER, 15);
	T_END();

	T_SCAN("%", TK_PERCENT);

	T_INIT("&&&f&");
	T_NEXT(TK_AMPERSHAND2);
	T_NEXT2(TK_NUMBER, 15);
	T_NEXT(TK_AMPERSHAND);
	T_END();

	T_SCAN("(", TK_LPAREN);
	T_SCAN(")", TK_RPAREN);

	T_INIT("***");
	T_NEXT(TK_STAR2);
	T_NEXT(TK_STAR);
	T_END();

	T_INIT("+++");
	T_NEXT(TK_PLUS2);
	T_NEXT(TK_PLUS);
	T_END();

	T_SCAN(",", TK_COMMA);

	T_INIT("---");
	T_NEXT(TK_MINUS2);
	T_NEXT(TK_MINUS);
	T_END();

	T_SCAN(".", TK_DOT);
	T_SCAN("/", TK_SLASH);
	T_SCAN(":", TK_COLON);

	T_INIT("<<<");
	T_NEXT(TK_LESS2);
	T_NEXT(TK_LESS);
	T_END();

	T_SCAN("<=", TK_LESS_EQ);
	T_SCAN("<>", TK_NOT_EQUAL);

	T_INIT("===");
	T_NEXT(TK_EQUAL2);
	T_NEXT(TK_EQUAL);
	T_END();

	T_INIT(">>>");
	T_NEXT(TK_GREATER2);
	T_NEXT(TK_GREATER);
	T_END();

	T_SCAN(">=", TK_GREATER_EQ);
	T_SCAN("?", TK_QUESTION);
	T_SCAN("[", TK_LSQUARE);
	T_SCAN("]", TK_RSQUARE);
	T_SCAN("^", TK_CARET);
	T_SCAN("{", TK_LCURLY);
	T_SCAN("}", TK_RCURLY);

	T_INIT("|||");
	T_NEXT(TK_VBAR2);
	T_NEXT(TK_VBAR);
	T_END();

	T_SCAN("~", TK_TILDE);

	T_SCAN("nz", TK_NZ);
	T_SCAN("z", TK_Z);
	T_SCAN("nc", TK_NC);
	T_SCAN("c", TK_C);
	T_SCAN("po", TK_PO);
	T_SCAN("pe", TK_PE);
	T_SCAN("p", TK_P);
	T_SCAN("m", TK_M);

	T_SCAN("Nz", TK_NZ);

	T_SCAN("b", TK_B);
	T_SCAN("c", TK_C);
	T_SCAN("d", TK_D);
	T_SCAN("e", TK_E);
	T_SCAN("h", TK_H);
	T_SCAN("l", TK_L);
	T_SCAN("a", TK_A);
	T_SCAN("i", TK_I);
	T_SCAN("r", TK_R);
	T_SCAN("ixh", TK_IXH);
	T_SCAN("ixl", TK_IXL);
	T_SCAN("iyh", TK_IYH);
	T_SCAN("iyl", TK_IYL);
	T_SCAN("bc", TK_BC);
	T_SCAN("de", TK_DE);
	T_SCAN("hl", TK_HL);
	T_SCAN("sp", TK_SP);
	T_SCAN("ix", TK_IX);
	T_SCAN("iy", TK_IY);

	T_INIT("af'af");
	T_NEXT(TK_AF);
	T_NEXT(TK_AF);
	T_END();

	T_SCAN2("adc", TK_IDENT, TK_ADC);
	T_SCAN2("add", TK_IDENT, TK_ADD);
	T_SCAN2("and", TK_IDENT, TK_AND);
	T_SCAN2("bit", TK_IDENT, TK_BIT);
	T_SCAN2("call", TK_IDENT, TK_CALL);
	T_SCAN2("ccf", TK_IDENT, TK_CCF);
	T_SCAN2("cp", TK_IDENT, TK_CP);
	T_SCAN2("cpd", TK_IDENT, TK_CPD);
	T_SCAN2("cpdr", TK_IDENT, TK_CPDR);
	T_SCAN2("cpi", TK_IDENT, TK_CPI);
	T_SCAN2("cpir", TK_IDENT, TK_CPIR);
	T_SCAN2("cpl", TK_IDENT, TK_CPL);
	T_SCAN2("daa", TK_IDENT, TK_DAA);
	T_SCAN2("dec", TK_IDENT, TK_DEC);
	T_SCAN2("di", TK_IDENT, TK_DI);
	T_SCAN2("djnz", TK_IDENT, TK_DJNZ);
	T_SCAN2("ei", TK_IDENT, TK_EI);
	T_SCAN2("ex", TK_IDENT, TK_EX);
	T_SCAN2("exx", TK_IDENT, TK_EXX);
	T_SCAN2("halt", TK_IDENT, TK_HALT);
	T_SCAN2("im", TK_IDENT, TK_IM);
	T_SCAN2("in", TK_IDENT, TK_IN);
	T_SCAN2("in0", TK_IDENT, TK_IN0);
	T_SCAN2("inc", TK_IDENT, TK_INC);
	T_SCAN2("ind", TK_IDENT, TK_IND);
	T_SCAN2("indr", TK_IDENT, TK_INDR);
	T_SCAN2("ini", TK_IDENT, TK_INI);
	T_SCAN2("inir", TK_IDENT, TK_INIR);
	T_SCAN2("jp", TK_IDENT, TK_JP);
	T_SCAN2("jr", TK_IDENT, TK_JR);
	T_SCAN2("ld", TK_IDENT, TK_LD);
	T_SCAN2("ldd", TK_IDENT, TK_LDD);
	T_SCAN2("lddr", TK_IDENT, TK_LDDR);
	T_SCAN2("ldi", TK_IDENT, TK_LDI);
	T_SCAN2("ldir", TK_IDENT, TK_LDIR);
	T_SCAN2("mlt", TK_IDENT, TK_MLT);
	T_SCAN2("neg", TK_IDENT, TK_NEG);
	T_SCAN2("nop", TK_IDENT, TK_NOP);
	T_SCAN2("or", TK_IDENT, TK_OR);
	T_SCAN2("otdm", TK_IDENT, TK_OTDM);
	T_SCAN2("otdmr", TK_IDENT, TK_OTDMR);
	T_SCAN2("otdr", TK_IDENT, TK_OTDR);
	T_SCAN2("otim", TK_IDENT, TK_OTIM);
	T_SCAN2("otimr", TK_IDENT, TK_OTIMR);
	T_SCAN2("otir", TK_IDENT, TK_OTIR);
	T_SCAN2("out", TK_IDENT, TK_OUT);
	T_SCAN2("out0", TK_IDENT, TK_OUT0);
	T_SCAN2("outd", TK_IDENT, TK_OUTD);
	T_SCAN2("outi", TK_IDENT, TK_OUTI);
	T_SCAN2("pop", TK_IDENT, TK_POP);
	T_SCAN2("push", TK_IDENT, TK_PUSH);
	T_SCAN2("res", TK_IDENT, TK_RES);
	T_SCAN2("ret", TK_IDENT, TK_RET);
	T_SCAN2("reti", TK_IDENT, TK_RETI);
	T_SCAN2("retn", TK_IDENT, TK_RETN);
	T_SCAN2("rl", TK_IDENT, TK_RL);
	T_SCAN2("rla", TK_IDENT, TK_RLA);
	T_SCAN2("rlc", TK_IDENT, TK_RLC);
	T_SCAN2("rlca", TK_IDENT, TK_RLCA);
	T_SCAN2("rld", TK_IDENT, TK_RLD);
	T_SCAN2("rr", TK_IDENT, TK_RR);
	T_SCAN2("rra", TK_IDENT, TK_RRA);
	T_SCAN2("rrc", TK_IDENT, TK_RRC);
	T_SCAN2("rrca", TK_IDENT, TK_RRCA);
	T_SCAN2("rrd", TK_IDENT, TK_RRD);
	T_SCAN2("rst", TK_IDENT, TK_RST);
	T_SCAN2("sbc", TK_IDENT, TK_SBC);
	T_SCAN2("scf", TK_IDENT, TK_SCF);
	T_SCAN2("set", TK_IDENT, TK_SET);
	T_SCAN2("sla", TK_IDENT, TK_SLA);
	T_SCAN2("sll", TK_IDENT, TK_SLL);
	T_SCAN2("slp", TK_IDENT, TK_SLP);
	T_SCAN2("sra", TK_IDENT, TK_SRA);
	T_SCAN2("srl", TK_IDENT, TK_SRL);
	T_SCAN2("sub", TK_IDENT, TK_SUB);
	T_SCAN2("tst", TK_IDENT, TK_TST);
	T_SCAN2("tstio", TK_IDENT, TK_TSTIO);
	T_SCAN2("xor", TK_IDENT, TK_XOR);

	T_SCAN2("binary", TK_IDENT, TK_BINARY);
	T_SCAN2("defb", TK_IDENT, TK_DEFB);
	T_SCAN2("defc", TK_IDENT, TK_DEFC);
	T_SCAN2("define", TK_IDENT, TK_DEFINE);
	T_SCAN2("defm", TK_IDENT, TK_DEFM);
	T_SCAN2("defq", TK_IDENT, TK_DEFQ);
	T_SCAN2("defs", TK_IDENT, TK_DEFS);
	T_SCAN2("defw", TK_IDENT, TK_DEFW);
	T_SCAN2("else", TK_IDENT, TK_ELSE);
	T_SCAN2("endif", TK_IDENT, TK_ENDIF);
	T_SCAN2("extern", TK_IDENT, TK_EXTERN);
	T_SCAN2("global", TK_IDENT, TK_GLOBAL);
	T_SCAN2("if", TK_IDENT, TK_IF);
	T_SCAN2("ifdef", TK_IDENT, TK_IFDEF);
	T_SCAN2("ifndef", TK_IDENT, TK_IFNDEF);
	T_SCAN2("line", TK_IDENT, TK_LINE);
	T_SCAN2("lston", TK_IDENT, TK_LSTON);
	T_SCAN2("lstoff", TK_IDENT, TK_LSTOFF);
	T_SCAN2("module", TK_IDENT, TK_MODULE);
	T_SCAN2("org", TK_IDENT, TK_ORG);
	T_SCAN2("public", TK_IDENT, TK_PUBLIC);
	T_SCAN2("section", TK_IDENT, TK_SECTION);
	T_SCAN2("title", TK_IDENT, TK_TITLE);
	T_SCAN2("undefine", TK_IDENT, TK_UNDEFINE);

	T_INIT("_Abc_123 Abc_123 123_Abc_0");
	T_NEXT3(TK_IDENT, 0, "_Abc_123");
	T_NEXT3(TK_IDENT, 0, "Abc_123");
	T_NEXT2(TK_NUMBER, 123);
	T_NEXT3(TK_IDENT, 0, "_Abc_0");
	T_END();
}

void test_quotes() {
	{
		START_CAPTURE();
		T_INIT("'hello");
		T_END();
		END_CAPTURE(
			"",
			"0: Error: missing closing quote\n"
			"\t'hello\n"
			"\t      ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("\"hello");
		T_END();
		END_CAPTURE(
			"",
			"0: Error: missing closing quote\n"
			"\t\"hello\n"
			"\t      ^~~~\n");
	}

	T_SCAN3("''", TK_STRING, 0, "");
	T_SCAN3("\"\"", TK_STRING, 0, "");

	T_SCAN3("'0'", TK_NUMBER, 48, "");
	T_SCAN3("\"0\"", TK_NUMBER, 48, "");

	T_SCAN3("'10'", TK_STRING, 0, "10");
	T_SCAN3("\"10\"", TK_STRING, 0, "10");

	T_SCAN3("'a'", TK_NUMBER, 'a', "");
	T_SCAN3("\"a\"", TK_NUMBER, 'a', "");

	T_SCAN3("'\\b'", TK_STRING, 0, "\\b");
	T_SCAN3("\"\\b\"", TK_NUMBER, '\b', "");

	T_SCAN3("'\\f'", TK_STRING, 0, "\\f");
	T_SCAN3("\"\\f\"", TK_NUMBER, '\f', "");

	T_SCAN3("'\\n'", TK_STRING, 0, "\\n");
	T_SCAN3("\"\\n\"", TK_NUMBER, '\n', "");

	T_SCAN3("'\\r'", TK_STRING, 0, "\\r");
	T_SCAN3("\"\\r\"", TK_NUMBER, '\r', "");

	T_SCAN3("'\\t'", TK_STRING, 0, "\\t");
	T_SCAN3("\"\\t\"", TK_NUMBER, '\t', "");

	T_SCAN3("'\\v'", TK_STRING, 0, "\\v");
	T_SCAN3("\"\\v\"", TK_NUMBER, '\v', "");

	T_SCAN3("'\\\\'", TK_STRING, 0, "\\\\");
	T_SCAN3("\"\\\\\"", TK_NUMBER, '\\', "");

	T_SCAN3("'\\'", TK_NUMBER, '\\', "");
	T_SCAN3("\"\\'\"", TK_NUMBER, '\'', "");

	T_SCAN3("'\\\"'", TK_STRING, 0, "\\\"");
	T_SCAN3("\"\\\"\"", TK_NUMBER, '"', "");

	T_SCAN3("'\\?'", TK_STRING, 0, "\\?");
	T_SCAN3("\"\\?\"", TK_NUMBER, '?', "");

	T_SCAN3("'\\0'", TK_STRING, 0, "\\0");
	T_SCAN3("\"\\0\"", TK_NUMBER, 0, "");

	T_SCAN3("'\\177'", TK_STRING, 0, "\\177");
	T_SCAN3("\"\\177\"", TK_NUMBER, 127, "");

	T_SCAN3("'\\x7f'", TK_STRING, 0, "\\x7f");
	T_SCAN3("\"\\x7f\"", TK_NUMBER, 127, "");

	T_SCAN3("\"a\\a\\b\\f\\n\\r\\t\\v\\\\\\'\\\"\\?\\1\\177\\x7f\"", TK_STRING, 0, "a\a\b\f\n\r\t\v\\'\"?\1\177\x7f");
	T_SCAN3("'a''b'", TK_STRING, 0, "a'b");
}

void test_include() {
	{
		START_CAPTURE();
		T_INIT("include");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: expected file\n"
			"\tinclude\n"
			"\t       ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include\n");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: expected file\n"
			"\tinclude\\\n"
			"\t       ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include;xx");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: expected file\n"
			"\tinclude;xx\n"
			"\t       ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include'");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: missing closing quote\n"
			"\tinclude'\n"
			"\t        ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include\"");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: missing closing quote\n"
			"\tinclude\"\n"
			"\t        ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include<");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: missing closing bracket\n"
			"\tinclude<\n"
			"\t        ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include''");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: expected file\n"
			"\tinclude''\n"
			"\t         ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include\"\"");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: expected file\n"
			"\tinclude\"\"\n"
			"\t         ^~~~\n");
	}
	{
		START_CAPTURE();
		T_INIT("include<>");
		T_NEXT2(TK_IDENT, TK_INCLUDE);
		OK(!scan.scan_filename());
		T_STR("", scan.text());
		T_END();
		END_CAPTURE(
			"",
			"0: Error: expected file\n"
			"\tinclude<>\n"
			"\t         ^~~~\n");
	}

	T_INIT(
		"include file1;comment\n"
		"include file2\n"
		"include \"file3\"\n"
		"include 'file4'\n"
		"include <file5>\n"
	);
	T_NEXT2(TK_IDENT, TK_INCLUDE);
	OK(scan.scan_filename()); T_STR("file1", scan.text());
	T_NEXT(TK_ENDL);

	T_NEXT2(TK_IDENT, TK_INCLUDE);
	OK(scan.scan_filename()); T_STR("file2", scan.text());
	T_NEXT(TK_ENDL);

	T_NEXT2(TK_IDENT, TK_INCLUDE);
	OK(scan.scan_filename()); T_STR("file3", scan.text());
	T_NEXT(TK_ENDL);

	T_NEXT2(TK_IDENT, TK_INCLUDE);
	OK(scan.scan_filename()); T_STR("file4", scan.text());
	T_NEXT(TK_ENDL);

	T_NEXT2(TK_IDENT, TK_INCLUDE);
	OK(scan.scan_filename()); T_STR("file5", scan.text());
	T_NEXT(TK_ENDL);

	T_END();
}

void test_numbers() {
	T_SCAN2("          0  ", TK_NUMBER, 0);
	T_SCAN2("          1  ", TK_NUMBER, 1);
	T_SCAN2("          9  ", TK_NUMBER, 9);
	T_SCAN2("         10  ", TK_NUMBER, 10);
	T_SCAN2("        011  ", TK_NUMBER, 11);			// not octal!
	T_SCAN2(" 2147483647  ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("          0d ", TK_NUMBER, 0);
	T_SCAN2("          1D ", TK_NUMBER, 1);
	T_SCAN2("          9d ", TK_NUMBER, 9);
	T_SCAN2("         10D ", TK_NUMBER, 10);
	T_SCAN2("         11d ", TK_NUMBER, 11);
	T_SCAN2(" 2147483647D ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("        0h ", TK_NUMBER, 0);
	T_SCAN2("        1H ", TK_NUMBER, 1);
	T_SCAN2("        9h ", TK_NUMBER, 9);
	T_SCAN2("       0EH ", TK_NUMBER, 14);
	T_SCAN2("       0fh ", TK_NUMBER, 15);
	T_SCAN2("       10H ", TK_NUMBER, 16);
	T_SCAN2("       11h ", TK_NUMBER, 17);
	T_SCAN2(" 7FffFFffH ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("        $0 ", TK_NUMBER, 0);
	T_SCAN2("        $1 ", TK_NUMBER, 1);
	T_SCAN2("        $9 ", TK_NUMBER, 9);
	T_SCAN2("        $E ", TK_NUMBER, 14);
	T_SCAN2("        $f ", TK_NUMBER, 15);
	T_SCAN2("       $10 ", TK_NUMBER, 16);
	T_SCAN2("       $11 ", TK_NUMBER, 17);
	T_SCAN2(" $7FffFFff ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("        #0 ", TK_NUMBER, 0);
	T_SCAN2("        #1 ", TK_NUMBER, 1);
	T_SCAN2("        #9 ", TK_NUMBER, 9);
	T_SCAN2("        #E ", TK_NUMBER, 14);
	T_SCAN2("        #f ", TK_NUMBER, 15);
	T_SCAN2("       #10 ", TK_NUMBER, 16);
	T_SCAN2("       #11 ", TK_NUMBER, 17);
	T_SCAN2(" #7FffFFff ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("        0x0 ", TK_NUMBER, 0);
	T_SCAN2("        0X1 ", TK_NUMBER, 1);
	T_SCAN2("        0x9 ", TK_NUMBER, 9);
	T_SCAN2("        0XE ", TK_NUMBER, 14);
	T_SCAN2("        0xf ", TK_NUMBER, 15);
	T_SCAN2("       0X10 ", TK_NUMBER, 16);
	T_SCAN2("       0x11 ", TK_NUMBER, 17);
	T_SCAN2(" 0X7FffFFff ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("        &x0 ", TK_NUMBER, 0);
	T_SCAN2("        &X1 ", TK_NUMBER, 1);
	T_SCAN2("        &x9 ", TK_NUMBER, 9);
	T_SCAN2("        &XE ", TK_NUMBER, 14);
	T_SCAN2("        &xf ", TK_NUMBER, 15);
	T_SCAN2("       &X10 ", TK_NUMBER, 16);
	T_SCAN2("       &x11 ", TK_NUMBER, 17);
	T_SCAN2(" &X7FffFFff ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("        &h0 ", TK_NUMBER, 0);
	T_SCAN2("        &H1 ", TK_NUMBER, 1);
	T_SCAN2("        &h9 ", TK_NUMBER, 9);
	T_SCAN2("        &HE ", TK_NUMBER, 14);
	T_SCAN2("        &hf ", TK_NUMBER, 15);
	T_SCAN2("       &H10 ", TK_NUMBER, 16);
	T_SCAN2("       &h11 ", TK_NUMBER, 17);
	T_SCAN2(" &H7FffFFff ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("         &0 ", TK_NUMBER, 0);
	T_SCAN2("         &1 ", TK_NUMBER, 1);
	T_SCAN2("         &9 ", TK_NUMBER, 9);
	T_SCAN2("         &E ", TK_NUMBER, 14);
	T_SCAN2("         &f ", TK_NUMBER, 15);
	T_SCAN2("        &10 ", TK_NUMBER, 16);
	T_SCAN2("        &11 ", TK_NUMBER, 17);
	T_SCAN2("  &7FffFFff ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("          0b ", TK_NUMBER, 0);
	T_SCAN2("          1B ", TK_NUMBER, 1);
	T_SCAN2("         10b ", TK_NUMBER, 2);
	T_SCAN2("         11B ", TK_NUMBER, 3);
	T_SCAN2(" 1111111111111111111111111111111b ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("          %0 ", TK_NUMBER, 0);
	T_SCAN2("          %1 ", TK_NUMBER, 1);
	T_SCAN2("         %10 ", TK_NUMBER, 2);
	T_SCAN2("         %11 ", TK_NUMBER, 3);
	T_SCAN2(" %1111111111111111111111111111111 ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("          @0 ", TK_NUMBER, 0);
	T_SCAN2("          @1 ", TK_NUMBER, 1);
	T_SCAN2("         @10 ", TK_NUMBER, 2);
	T_SCAN2("         @11 ", TK_NUMBER, 3);
	T_SCAN2(" @1111111111111111111111111111111 ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("          0b0 ", TK_NUMBER, 0);
	T_SCAN2("          0B1 ", TK_NUMBER, 1);
	T_SCAN2("         0b10 ", TK_NUMBER, 2);
	T_SCAN2("         0B11 ", TK_NUMBER, 3);
	T_SCAN2(" 0b1111111111111111111111111111111 ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("          &b0 ", TK_NUMBER, 0);
	T_SCAN2("          &B1 ", TK_NUMBER, 1);
	T_SCAN2("         &b10 ", TK_NUMBER, 2);
	T_SCAN2("         &B11 ", TK_NUMBER, 3);
	T_SCAN2(" &b1111111111111111111111111111111 ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("           0q ", TK_NUMBER, 0);
	T_SCAN2("           1Q ", TK_NUMBER, 1);
	T_SCAN2("           7o ", TK_NUMBER, 7);
	T_SCAN2("          10O ", TK_NUMBER, 8);
	T_SCAN2("          11q ", TK_NUMBER, 9);
	T_SCAN2(" 17777777777Q ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("           0q0 ", TK_NUMBER, 0);
	T_SCAN2("           0Q1 ", TK_NUMBER, 1);
	T_SCAN2("           0o7 ", TK_NUMBER, 7);
	T_SCAN2("          0O10 ", TK_NUMBER, 8);
	T_SCAN2("          0q11 ", TK_NUMBER, 9);
	T_SCAN2(" 0Q17777777777 ", TK_NUMBER, 0x7FFFFFFF);

	T_SCAN2("           &q0 ", TK_NUMBER, 0);
	T_SCAN2("           &Q1 ", TK_NUMBER, 1);
	T_SCAN2("           &o7 ", TK_NUMBER, 7);
	T_SCAN2("          &O10 ", TK_NUMBER, 8);
	T_SCAN2("          &q11 ", TK_NUMBER, 9);
	T_SCAN2(" &Q17777777777 ", TK_NUMBER, 0x7FFFFFFF);
}

int main()
{
	START_TESTING();

	test_before_init();
	test_back_and_forward();
	test_comments_and_backslash();
	test_errors();
	test_tokens();
	test_quotes();
	test_include();
	test_numbers();

	DONE_TESTING();
}
