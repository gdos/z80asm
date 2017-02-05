//-----------------------------------------------------------------------------
// z80asm scanner
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "scanner.h"
#include "message.h"
#include "source.h"
#include <cassert>
#include <cstdlib>
#include <climits>

Token Scanner::eoi_token = Token(TK_EOI);
SrcLine Scanner::default_line = SrcLine(NULL, 0, "");

Scanner::Scanner()
	: line_(NULL), p_(NULL), marker_(NULL), ctxmarker_(NULL)
	, tok_p_(0) {
	init(&default_line);
}

Scanner::~Scanner() {
}

void Scanner::init(SrcLine* line) {
	line_ = line;
	p_ = line->text().c_str();
	marker_ = ctxmarker_ = NULL;
	tokens_.clear();
	tok_p_ = 0;
}

Token* Scanner::peek(int n) {
	assert(n >= 0);

	// fill buffer
	while (tok_p_ + n >= tokens_.size()) {
		if (!push_next())
			return &eoi_token;
	}

	return &tokens_[tok_p_ + n];
}

Token* Scanner::next() {
	Token* tok = peek();
	if (tok_p_ < tokens_.size())
		++tok_p_;
	return tok;
}

int Scanner::get_pos() const {
	return tok_p_;
}

void Scanner::set_pos(int pos) {
	assert(pos >= 0);
	assert(pos <= static_cast<int>(tokens_.size()));
	tok_p_ = pos;
}

void Scanner::error(void(*errfunc)(SrcLine*, int)) {
	int column = (line_ == NULL || p_ == NULL) ? 0 : p_ - line_->text().c_str() + 1;
	errfunc(line_, column);
	flush();
}

void Scanner::warning(void(*errfunc)(SrcLine*, int)) {
	int column = (line_ == NULL || p_ == NULL) ? 0 : p_ - line_->text().c_str() + 1;
	errfunc(line_, column);
}

int Scanner::number(const char* ts, int base) {
	unsigned long value = std::strtoul(ts, NULL, base);
	return static_cast<int>(value);
}

/*!re2c
	re2c:define:YYCTYPE = char;
	re2c:define:YYCURSOR = p_;
	re2c:define:YYMARKER = marker_;
	re2c:define:YYCTXMARKER = ctxmarker_;
	re2c:yyfill:enable = 0;
	re2c:indent:top = 2;

	end		= [\x00];
	space	= [ \t\v\r\n\f];
	alpha	= [a-zA-Z_];
	alnum	= [a-zA-Z_0-9];
	b		= [^a-zA-Z_0-9];
	dec		= [0-9];
	oct		= [0-7];
	hex		= [0-9a-fA-F];
	bin		= [01];
*/

void Scanner::flush() {
	for (;;) {
		/*!re2c
			*				{ continue; }
			end				{ --p_; return; }
			'\n'			{ return; }
		*/
	}
}

#define PUSH(a) 			tokens_.push_back(Token(a)); return true;
#define PUSH2(a,b) 			tokens_.push_back(Token(a,b)); return true;
#define PUSH4(a,b,c,d) 		tokens_.push_back(Token(a,b,c,d)); return true;
#define PUSH_NUM(ts,base)	tokens_.push_back(Token(TK_NUMBER, number(ts, base))); return true;

bool Scanner::push_next() {
	const char* ts;

	for (;;) {
		ts = p_;
		/*!re2c
			*				{ --p_; error(err::syntax); return false; }
			end				{ --p_; return false; }
			';'				{ flush(); PUSH(TK_ENDL); }
			'\n' | '\\'		{ PUSH(TK_ENDL); }
			space			{ continue; }
			'!'				{ PUSH(TK_EXCLAM); }
			'#'				{ PUSH(TK_HASH); }
			'##'			{ PUSH(TK_HASH2); }
			'%'				{ PUSH(TK_PERCENT); }
			'&'				{ PUSH(TK_AMPERSHAND); }
			'&&'			{ PUSH(TK_AMPERSHAND2); }
			'('				{ PUSH(TK_LPAREN); }
			')'				{ PUSH(TK_RPAREN); }
			'*'				{ PUSH(TK_STAR); }
			'**'			{ PUSH(TK_STAR2); }
			'+'				{ PUSH(TK_PLUS); }
			'++'			{ PUSH(TK_PLUS2); }
			','				{ PUSH(TK_COMMA); }
			'-'				{ PUSH(TK_MINUS); }
			'--'			{ PUSH(TK_MINUS2); }
			'.'				{ PUSH(TK_DOT); }
			'/'				{ PUSH(TK_SLASH); }
			':'				{ PUSH(TK_COLON); }
			'<'				{ PUSH(TK_LESS); }
			'<<'			{ PUSH(TK_LESS2); }
			'<='			{ PUSH(TK_LESS_EQ); }
			'<>' | '!='		{ PUSH(TK_NOT_EQUAL); }
			'='				{ PUSH(TK_EQUAL); }
			'=='			{ PUSH(TK_EQUAL2); }
			'>'				{ PUSH(TK_GREATER); }
			'>>'			{ PUSH(TK_GREATER2); }
			'>='			{ PUSH(TK_GREATER_EQ); }
			'?'				{ PUSH(TK_QUESTION); }
			'['				{ PUSH(TK_LSQUARE); }
			']'				{ PUSH(TK_RSQUARE); }
			'^'				{ PUSH(TK_CARET); }
			'{'				{ PUSH(TK_LCURLY); }
			'}'				{ PUSH(TK_RCURLY); }
			'|'				{ PUSH(TK_VBAR); }
			'||'			{ PUSH(TK_VBAR2); }
			'~'				{ PUSH(TK_TILDE); }

			'NZ'	/b		{ PUSH(TK_NZ); }
			'Z'		/b		{ PUSH(TK_Z); }
			'NC'	/b		{ PUSH(TK_NC); }
			'C'		/b		{ PUSH(TK_C); }
			'PO'	/b		{ PUSH(TK_PO); }
			'PE'	/b		{ PUSH(TK_PE); }
			'P'		/b		{ PUSH(TK_P); }
			'M'		/b		{ PUSH(TK_M); }
			
			'B'		/b		{ PUSH(TK_B); }
			'D'		/b		{ PUSH(TK_D); }
			'E'		/b		{ PUSH(TK_E); }
			'H'		/b		{ PUSH(TK_H); }
			'L'		/b		{ PUSH(TK_L); }
			'A'		/b		{ PUSH(TK_A); }
			'F'		/b		{ PUSH(TK_F); }
			'I'		/b		{ PUSH(TK_I); }
			'R'		/b		{ PUSH(TK_R); }
			'IXH'	/b		{ PUSH(TK_IXH); }
			'IXL'	/b		{ PUSH(TK_IXL); }
			'IYH'	/b		{ PUSH(TK_IYH); }
			'IYL'	/b		{ PUSH(TK_IYL); }

			'BC'	/b		{ PUSH(TK_BC); }
			'DE'	/b		{ PUSH(TK_DE); }
			'HL'	/b		{ PUSH(TK_HL); }
			'AF' "'"		{ PUSH(TK_AF); }
			'AF'	/b		{ PUSH(TK_AF); }
			'SP'	/b		{ PUSH(TK_SP); }
			'IX'	/b		{ PUSH(TK_IX); }
			'IY'	/b		{ PUSH(TK_IY); }

			'ASMPC' /b		{ PUSH(TK_ASMPC); }
			'$'				{ PUSH(TK_ASMPC); }
			
			'ADC'	/b		{ PUSH2(TK_IDENT, TK_ADC); }
			'ADD'	/b		{ PUSH2(TK_IDENT, TK_ADD); }
			'AND'	/b		{ PUSH2(TK_IDENT, TK_AND); }
			'BIT'	/b		{ PUSH2(TK_IDENT, TK_BIT); }
			'CALL'	/b		{ PUSH2(TK_IDENT, TK_CALL); }
			'CCF'	/b		{ PUSH2(TK_IDENT, TK_CCF); }
			'CP'	/b		{ PUSH2(TK_IDENT, TK_CP); }
			'CPD'	/b		{ PUSH2(TK_IDENT, TK_CPD); }
			'CPDR'	/b		{ PUSH2(TK_IDENT, TK_CPDR); }
			'CPI'	/b		{ PUSH2(TK_IDENT, TK_CPI); }
			'CPIR'	/b		{ PUSH2(TK_IDENT, TK_CPIR); }
			'CPL'	/b		{ PUSH2(TK_IDENT, TK_CPL); }
			'DAA'	/b		{ PUSH2(TK_IDENT, TK_DAA); }
			'DEC'	/b		{ PUSH2(TK_IDENT, TK_DEC); }
			'DI'	/b		{ PUSH2(TK_IDENT, TK_DI); }
			'DJNZ'	/b		{ PUSH2(TK_IDENT, TK_DJNZ); }
			'EI'	/b		{ PUSH2(TK_IDENT, TK_EI); }
			'EX'	/b		{ PUSH2(TK_IDENT, TK_EX); }
			'EXX'	/b		{ PUSH2(TK_IDENT, TK_EXX); }
			'HALT'	/b		{ PUSH2(TK_IDENT, TK_HALT); }
			'IM'	/b		{ PUSH2(TK_IDENT, TK_IM); }
			'IN'	/b		{ PUSH2(TK_IDENT, TK_IN); }
			'IN0'	/b		{ PUSH2(TK_IDENT, TK_IN0); }
			'INC'	/b		{ PUSH2(TK_IDENT, TK_INC); }
			'IND'	/b		{ PUSH2(TK_IDENT, TK_IND); }
			'INDR'	/b		{ PUSH2(TK_IDENT, TK_INDR); }
			'INI'	/b		{ PUSH2(TK_IDENT, TK_INI); }
			'INIR'	/b		{ PUSH2(TK_IDENT, TK_INIR); }
			'JP'	/b		{ PUSH2(TK_IDENT, TK_JP); }
			'JR'	/b		{ PUSH2(TK_IDENT, TK_JR); }
			'LD'	/b		{ PUSH2(TK_IDENT, TK_LD); }
			'LDD'	/b		{ PUSH2(TK_IDENT, TK_LDD); }
			'LDDR'	/b		{ PUSH2(TK_IDENT, TK_LDDR); }
			'LDI'	/b		{ PUSH2(TK_IDENT, TK_LDI); }
			'LDIR'	/b		{ PUSH2(TK_IDENT, TK_LDIR); }
			'MLT'	/b		{ PUSH2(TK_IDENT, TK_MLT); }
			'NEG'	/b		{ PUSH2(TK_IDENT, TK_NEG); }
			'NOP'	/b		{ PUSH2(TK_IDENT, TK_NOP); }
			'OR'	/b		{ PUSH2(TK_IDENT, TK_OR); }
			'OTDM'	/b		{ PUSH2(TK_IDENT, TK_OTDM); }
			'OTDMR'	/b		{ PUSH2(TK_IDENT, TK_OTDMR); }
			'OTDR'	/b		{ PUSH2(TK_IDENT, TK_OTDR); }
			'OTIM'	/b		{ PUSH2(TK_IDENT, TK_OTIM); }
			'OTIMR'	/b		{ PUSH2(TK_IDENT, TK_OTIMR); }
			'OTIR'	/b		{ PUSH2(TK_IDENT, TK_OTIR); }
			'OUT'	/b		{ PUSH2(TK_IDENT, TK_OUT); }
			'OUT0'	/b		{ PUSH2(TK_IDENT, TK_OUT0); }
			'OUTD'	/b		{ PUSH2(TK_IDENT, TK_OUTD); }
			'OUTI'	/b		{ PUSH2(TK_IDENT, TK_OUTI); }
			'POP'	/b		{ PUSH2(TK_IDENT, TK_POP); }
			'PUSH'	/b		{ PUSH2(TK_IDENT, TK_PUSH); }
			'RES'	/b		{ PUSH2(TK_IDENT, TK_RES); }
			'RET'	/b		{ PUSH2(TK_IDENT, TK_RET); }
			'RETI'	/b		{ PUSH2(TK_IDENT, TK_RETI); }
			'RETN'	/b		{ PUSH2(TK_IDENT, TK_RETN); }
			'RL'	/b		{ PUSH2(TK_IDENT, TK_RL); }
			'RLA'	/b		{ PUSH2(TK_IDENT, TK_RLA); }
			'RLC'	/b		{ PUSH2(TK_IDENT, TK_RLC); }
			'RLCA'	/b		{ PUSH2(TK_IDENT, TK_RLCA); }
			'RLD'	/b		{ PUSH2(TK_IDENT, TK_RLD); }
			'RR'	/b		{ PUSH2(TK_IDENT, TK_RR); }
			'RRA'	/b		{ PUSH2(TK_IDENT, TK_RRA); }
			'RRC'	/b		{ PUSH2(TK_IDENT, TK_RRC); }
			'RRCA'	/b		{ PUSH2(TK_IDENT, TK_RRCA); }
			'RRD'	/b		{ PUSH2(TK_IDENT, TK_RRD); }
			'RST'	/b		{ PUSH2(TK_IDENT, TK_RST); }
			'SBC'	/b		{ PUSH2(TK_IDENT, TK_SBC); }
			'SCF'	/b		{ PUSH2(TK_IDENT, TK_SCF); }
			'SET'	/b		{ PUSH2(TK_IDENT, TK_SET); }
			'SLA'	/b		{ PUSH2(TK_IDENT, TK_SLA); }
			'SLL'	/b		{ PUSH2(TK_IDENT, TK_SLL); }
			'SLP'	/b		{ PUSH2(TK_IDENT, TK_SLP); }
			'SRA'	/b		{ PUSH2(TK_IDENT, TK_SRA); }
			'SRL'	/b		{ PUSH2(TK_IDENT, TK_SRL); }
			'SUB'	/b		{ PUSH2(TK_IDENT, TK_SUB); }
			'TST'	/b		{ PUSH2(TK_IDENT, TK_TST); }
			'TSTIO'	/b		{ PUSH2(TK_IDENT, TK_TSTIO); }
			'XOR'	/b		{ PUSH2(TK_IDENT, TK_XOR); }

			'BINARY'	/b	{ PUSH2(TK_IDENT, TK_BINARY); }
			'DEFB'		/b	{ PUSH2(TK_IDENT, TK_DEFB); }
			'DEFC'		/b	{ PUSH2(TK_IDENT, TK_DEFC); }
			'DEFINE'	/b	{ PUSH2(TK_IDENT, TK_DEFINE); }
			'DEFM'		/b	{ PUSH2(TK_IDENT, TK_DEFM); }
			'DEFQ'		/b	{ PUSH2(TK_IDENT, TK_DEFQ); }
			'DEFS'		/b	{ PUSH2(TK_IDENT, TK_DEFS); }
			'DEFW'		/b	{ PUSH2(TK_IDENT, TK_DEFW); }
			'ELSE'		/b	{ PUSH2(TK_IDENT, TK_ELSE); }
			'ENDIF'		/b	{ PUSH2(TK_IDENT, TK_ENDIF); }
			'EXTERN'	/b	{ PUSH2(TK_IDENT, TK_EXTERN); }
			'GLOBAL'	/b	{ PUSH2(TK_IDENT, TK_GLOBAL); }
			'IF'		/b	{ PUSH2(TK_IDENT, TK_IF); }
			'IFDEF'		/b	{ PUSH2(TK_IDENT, TK_IFDEF); }
			'IFNDEF'	/b	{ PUSH2(TK_IDENT, TK_IFNDEF); }
			'INCLUDE'	/b	{ PUSH2(TK_IDENT, TK_INCLUDE); }
			'LINE'		/b	{ PUSH2(TK_IDENT, TK_LINE); }
			'LSTON'		/b	{ PUSH2(TK_IDENT, TK_LSTON); }
			'LSTOFF'	/b	{ PUSH2(TK_IDENT, TK_LSTOFF); }
			'MODULE'	/b	{ PUSH2(TK_IDENT, TK_MODULE); }
			'ORG'		/b	{ PUSH2(TK_IDENT, TK_ORG); }
			'PUBLIC'	/b	{ PUSH2(TK_IDENT, TK_PUBLIC); }
			'SECTION'	/b	{ PUSH2(TK_IDENT, TK_SECTION); }
			'TITLE'		/b	{ PUSH2(TK_IDENT, TK_TITLE); }
			'UNDEFINE'	/b	{ PUSH2(TK_IDENT, TK_UNDEFINE); }
			
			alpha alnum*	{ PUSH4(TK_IDENT, 0, ts, p_); }

			[$#] hex+		{ PUSH_NUM(ts + 1, 16); }
			[@%] bin+		{ PUSH_NUM(ts + 1, 2); }

			'&h' hex+		{ PUSH_NUM(ts + 2, 16); }
			'&x' hex+		{ PUSH_NUM(ts + 2, 16); }
			'&o' oct+		{ PUSH_NUM(ts + 2, 8); }
			'&q' oct+		{ PUSH_NUM(ts + 2, 8); }
			'&b' bin+		{ PUSH_NUM(ts + 2, 2); }
			'&'  hex+		{ PUSH_NUM(ts + 1, 16); }

			'0x' hex+		{ PUSH_NUM(ts + 2, 16); }
			'0q' oct+		{ PUSH_NUM(ts + 2, 8); }
			'0o' oct+		{ PUSH_NUM(ts + 2, 8); }
			'0b' bin+		{ PUSH_NUM(ts + 2, 2); }

			dec hex* 'h' 	{ PUSH_NUM(ts, 16); }

			oct+ 'o'		{ PUSH_NUM(ts, 8); }
			oct+ 'q'		{ PUSH_NUM(ts, 8); }

			bin+ 'b'		{ PUSH_NUM(ts, 2); }

			dec+ 'd'?		{ PUSH_NUM(ts, 10); }

			"'"				{ text_.clear(); goto squote; }
			'"'				{ text_.clear(); goto dquote; }
		*/
	}
	assert(0);				// not reached

	// Single-quoted string: does not interpret C-escapes, '' is one quote
squote:
	for (;;) {
		/*!re2c
			*				{ --p_; error(err::missing_closing_quote); return false; }
			"''"			{ text_.push_back('\''); continue; }
			"'"				{ if (text_.length() == 1)	tokens_.push_back(Token(TK_NUMBER, text_[0])); 
							  else						tokens_.push_back(Token(TK_STRING, 0, 
																	text_.c_str(), text_.c_str()+text_.length())); 
							  return true; }
		  [^\x00\n]			{ text_.push_back(yych); continue; }
		*/
	}
	assert(0);				// not reached

	// Double-quoted string: interprets C-escapes
dquote:
	for (;;) {
		ts = p_;
		/*!re2c
			*				{ --p_; error(err::missing_closing_quote); return false; }
			'"'				{ if (text_.length() == 1)	tokens_.push_back(Token(TK_NUMBER, text_[0])); 
							  else						tokens_.push_back(Token(TK_STRING, 0, 
																	text_.c_str(), text_.c_str()+text_.length())); 
							  return true; }
			"\\x" hex+		{ text_.push_back(number(ts + 2, 16)); continue; }
			'\\' oct+		{ text_.push_back(number(ts + 1, 8)); continue; }
			"\\a"			{ text_.push_back('\a'); continue; }
			"\\b"			{ text_.push_back('\b'); continue; }
			"\\f"			{ text_.push_back('\f'); continue; }
			"\\n"			{ text_.push_back('\n'); continue; }
			"\\r"			{ text_.push_back('\r'); continue; }
			"\\t"			{ text_.push_back('\t'); continue; }
			"\\v"			{ text_.push_back('\v'); continue; }
			'\\\\'			{ text_.push_back('\\'); continue; }
			"\\'"			{ text_.push_back('\''); continue; }
			'\\"'			{ text_.push_back('"'); continue; }
			'\\?'			{ text_.push_back('?'); continue; }
			[^\x00\n]		{ text_.push_back(yych); continue; }
		*/
	}
	assert(0);				// not reached

	return false;			// keep compiler happy
}

bool Scanner::scan_filename() {
	const char* ts;

	text_.clear();
	for (;;) {
		/*!re2c
			*				{ ts = p_-1; goto word; }
			end|'\n'|';'	{ p_--; goto end; }
			'<'				{ ts = p_; goto angles; }
			'"'				{ ts = p_; goto dquote; }
			"'"				{ ts = p_; goto squote; }
			space			{ continue; }
		*/
	}
	assert(0);				// not reached

word:
	for (;;) {
		/*!re2c
			*					{ continue; }
			end|'\n'|';'|space	{ p_--; text_ = std::string(ts, p_); goto end; }
		*/
	}
	assert(0);				// not reached

angles:
	for (;;) {
		/*!re2c
			*				{ continue; }
			end | '\n'		{ --p_; error(err::missing_closing_bracket); return false; }
			'>'				{ text_ = std::string(ts, p_-1); goto end; }
		*/
	}
	assert(0);				// not reached

dquote:
	for (;;) {
		/*!re2c
			*				{ continue; }
			end | '\n'		{ --p_; error(err::missing_closing_quote); return false; }
			'"'				{ text_ = std::string(ts, p_-1); goto end; }
		*/
	}
	assert(0);				// not reached

squote:
	for (;;) {
		/*!re2c
			*				{ continue; }
			end | '\n'		{ --p_; error(err::missing_closing_quote); return false; }
			"'"				{ text_ = std::string(ts, p_-1); goto end; }
		*/
	}
	assert(0);				// not reached

end:
	if (text_.empty()) {
		error(err::expected_file); 
		return false;
	}
	else {
		return true;
	}
}

#undef PUSH
#undef PUSH2
#undef PUSH4
#undef PUSH_NUM

