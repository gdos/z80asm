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

bool Scanner::push_next() {
	int hash; 				// build hash of identifiers
	int count;
	bool squote;
	const char* ts;

	for (;;) {
		ts = p_;
		/*!re2c
			*				{ --p_; error(err::syntax); return false; }
			end				{ --p_; return false; }
			';'				{ tokens_.push_back(Token(TK_ENDL)); flush(); return true; }
			'\n' | '\\'		{ tokens_.push_back(Token(TK_ENDL)); return true; }
			space			{ continue; }
			'!'				{ tokens_.push_back(Token(TK_EXCLAM)); return true; }
			'#'				{ tokens_.push_back(Token(TK_HASH)); return true; }
			'%'				{ tokens_.push_back(Token(TK_PERCENT)); return true; }
			'&'				{ tokens_.push_back(Token(TK_AMPERSHAND)); return true; }
			'&&'			{ tokens_.push_back(Token(TK_AMPERSHAND2)); return true; }
			'('				{ tokens_.push_back(Token(TK_LPAREN)); return true; }
			')'				{ tokens_.push_back(Token(TK_RPAREN)); return true; }
			'*'				{ tokens_.push_back(Token(TK_STAR)); return true; }
			'**'			{ tokens_.push_back(Token(TK_STAR2)); return true; }
			'+'				{ tokens_.push_back(Token(TK_PLUS)); return true; }
			'++'			{ tokens_.push_back(Token(TK_PLUS2)); return true; }
			','				{ tokens_.push_back(Token(TK_COMMA)); return true; }
			'-'				{ tokens_.push_back(Token(TK_MINUS)); return true; }
			'--'			{ tokens_.push_back(Token(TK_MINUS2)); return true; }
			'.'				{ tokens_.push_back(Token(TK_DOT)); return true; }
			'/'				{ tokens_.push_back(Token(TK_SLASH)); return true; }
			':'				{ tokens_.push_back(Token(TK_COLON)); return true; }
			'<'				{ tokens_.push_back(Token(TK_LESS)); return true; }
			'<<'			{ tokens_.push_back(Token(TK_LESS2)); return true; }
			'<='			{ tokens_.push_back(Token(TK_LESS_EQ)); return true; }
			'<>' | '!='		{ tokens_.push_back(Token(TK_NOT_EQUAL)); return true; }
			'='				{ tokens_.push_back(Token(TK_EQUAL)); return true; }
			'=='			{ tokens_.push_back(Token(TK_EQUAL2)); return true; }
			'>'				{ tokens_.push_back(Token(TK_GREATER)); return true; }
			'>>'			{ tokens_.push_back(Token(TK_GREATER2)); return true; }
			'>='			{ tokens_.push_back(Token(TK_GREATER_EQ)); return true; }
			'?'				{ tokens_.push_back(Token(TK_QUESTION)); return true; }
			'['				{ tokens_.push_back(Token(TK_LSQUARE)); return true; }
			']'				{ tokens_.push_back(Token(TK_RSQUARE)); return true; }
			'^'				{ tokens_.push_back(Token(TK_CARET)); return true; }
			'{'				{ tokens_.push_back(Token(TK_LCURLY)); return true; }
			'}'				{ tokens_.push_back(Token(TK_RCURLY)); return true; }
			'|'				{ tokens_.push_back(Token(TK_VBAR)); return true; }
			'||'			{ tokens_.push_back(Token(TK_VBAR2)); return true; }
			'~'				{ tokens_.push_back(Token(TK_TILDE)); return true; }

			alpha			{ hash = ALNUM_HASH(yych); count = 1; goto ident; }

			dec hex* 'h'	{ tokens_.push_back(Token(TK_NUMBER, number(ts, 16))); return true; }
			'$' hex+		{ tokens_.push_back(Token(TK_NUMBER, number(ts + 1, 16))); return true; }
			'0x' hex+		{ tokens_.push_back(Token(TK_NUMBER, number(ts + 2, 16))); return true; }

			bin+ 'b'		{ tokens_.push_back(Token(TK_NUMBER, number(ts, 2))); return true; }
			[@%] bin+		{ tokens_.push_back(Token(TK_NUMBER, number(ts + 1, 2))); return true; }
			'0b' bin+		{ tokens_.push_back(Token(TK_NUMBER, number(ts + 2, 2))); return true; }
			
			dec+			{ tokens_.push_back(Token(TK_NUMBER, number(ts, 10))); return true; }

			"'"				{ text_.clear(); squote = true;  goto quote; }
			'"'				{ text_.clear(); squote = false; goto quote; }
		*/
	}
	assert(0);				// not reached

ident:
	for (;;) {
		/*!re2c
			*				{ if (!(yych == '\'' && count == 2 && hash == KW_AF)) --p_;
							  tokens_.push_back(Token(TK_IDENT, hash, ts, p_)); return true; }
			alnum			{ if (++count < 6) { hash += ALNUM_HASH(yych) << ((count - 1) * 6); } continue; }
		*/
	}
	assert(0);				// not reached

quote:
	for (;;) {
		ts = p_;
		/*!re2c
			*				{ --p_; error(err::missing_closing_quote); return false; }
			"'"				{ if (squote) {
								if (text_.length() != 1) { error(err::squoted_string); return false; }
								else { tokens_.push_back(Token(TK_NUMBER, text_[0])); return true; }
							  }
							  else { text_.push_back(yych); continue; }
							}
			'"'				{ if (!squote) {
								tokens_.push_back(Token(TK_STRING, 0, text_.c_str(), text_.c_str()+text_.length())); return true;
							  }
							  else { text_.push_back(yych); continue; }
							}
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