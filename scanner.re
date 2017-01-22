//-----------------------------------------------------------------------------
// z80asm scanner
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "scanner.h"
#include "message.h"
#include "object.h"
#include "opcode.h"
#include "parser.h"
#include "source.h"
#include <cassert>

Scanner::Scanner()
	: line_(NULL), text_(""), number_(0), opcode_(NULL)
	, ts_(NULL), p_(NULL)
	, marker_(NULL), ctxmarker_(NULL) {}

Scanner::~Scanner() {
}

void Scanner::init(SrcLine* line) {
	line_ = line;
	ts_ = p_ = line->text().c_str();
	marker_ = ctxmarker_ = NULL;
}

/*!re2c
	re2c:define:YYCTYPE = char;
	re2c:define:YYCURSOR = p_;
	re2c:define:YYMARKER = marker_;
	re2c:define:YYCTXMARKER = ctxmarker_;
	re2c:yyfill:enable = 0;

	end		=  [\x00];
	s		=  [ \t\v\r\n\f];
	w		=  [a-zA-Z0-9_];
	nw		= [^a-zA-Z0-9_];
	ident	=  [a-zA-Z_] w*;

*/

int Scanner::get_opcode() {
	if (!line_)
		return EOI;

	for (;;) {
		/*!re2c
			*				{ --p_; error(err::syntax); return EOI; }
			end | ';'		{ return EOI; }
			s+				{ continue; }

			'INCLUDE' /nw	{ return TK_INCLUDE; }
			
			'NOP' /nw		{ opcode_ = Opcode::nop(line_); return TK_OPCODE_VOID; }
		*/
	}
}

bool Scanner::get_filename() {
	if (!line_)
		return false;

	for (;;) {
		/*!re2c
			*				{ --p_; error(err::expected_file); return false; }
			s+				{ continue; }
			'"'				{ break; }
		*/
	}

	const char* file_start = p_;
	for (;;) {
		/*!re2c
			*				{ --p_; error(err::expected_quotes); return false; }
			'"'				{ break; }
			[\x20-\xFF]		{ continue; }
		*/
	}
	const char* file_end = p_ - 1;
	text_ = std::string(file_start, file_end);
	return true;
}

bool Scanner::get_end_statement() {
	if (!line_)
		return false;

	for (;;) {
		/*!re2c
		*				{ --p_; error(err::expected_end_of_statement); return false; }
		s+				{ continue; }
		end				{ --p_; return true; }
		';'				{ flush(); return true; }
		'\\'			{ return true; }
		*/
	}
	return false;	// not reached
}

void Scanner::flush() {
	if (line_) {
		p_ = ts_ + line_->text().length();
	}
}


void Scanner::error(void(*errfunc)(SrcLine*, int)) {
	if (line_) {
		errfunc(from(), column());
		flush();
	}
}

#if 0

#define GOTO(state_) state = yyc##state_; goto yyc_##state_

bool Scanner::scan_line(SrcLine* line) {
	const char*	YYSTART = line->text().c_str();
	const char*	YYCURSOR = YYSTART;
	const char* YYTOKEN;
	const char* YYCTXMARKER;
	std::string file;
	int state = yycstart;
	std::vector<int> states;

	for (;;) {
		/*  !re2c
			re2c:define:YYCTYPE = char;
			re2c:yyfill:enable = 0;
			re2c:define:YYGETCONDITION = "state";
			re2c:define:YYGETCONDITION:naked = 1;
			re2c:define:YYSETCONDITION = "state = @@;";
			re2c:define:YYSETCONDITION:naked = 1;
			re2c:indent:top = 2;

			end		=  [\x00];
			not_end	= [^\x00];
			ns		= [^ \t\v\r\n\f];
			comment	= ';' not_end*;
			end_stmt= s* comment?;

			<*>				*				{ --YYCURSOR; 
											  err::syntax(line, YYCURSOR - YYSTART + 1); 
											  return false; }
			<*>				s+				{ continue; }

			<start, end>	end | ';'		{ return true; }

			<start>			s* 'INCLUDE' /nw { GOTO(include); }

			<include>		s* '"'			{ YYTOKEN = YYCURSOR; GOTO(include_file); }
			<include_file>	[^"\x00]+ '"'	{ file = std::string(YYTOKEN, YYCURSOR); 
											  GOTO(end); }
		
		*/
	}
	return true;	// not reached
}


#if 0



#include "token.h"
#include <cassert>


struct ScanPos {
	SrcLine*	line;
	const char*	ts;
	const char*	p;
	const char* token;
	const char* marker;
	const char* ctxmarker;

	ScanPos(SrcLine* line_) : line(line_), ts(), p(ts), token(NULL), marker(NULL), ctxmarker(NULL) {}
};

static bool scan_label(ScanPos* pos, Parser* parser) { return true; }
static bool get_opcode(ScanPos* pos, Parser* parser) { return true; }

static bool do_include(ScanPos* pos, Parser* parser) {
	const char* file_s = strchr(pos->token, '"'); assert(file_s);
	const char* file_e = strchr(file_s+1, '"'); assert(file_e);
	std::string file(file_s, file_e);
	return parser->object()->open(file);
}

#define PUSH(t)				parser->push(t);
#define SYNTAX_ERROR()		

static bool scan_line(ScanPos* pos, Parser* parser) {
	bool ok = true;
	while (ok) {
		pos->token = pos->p;
	}

	return ok;
}



bool scan_line(SrcLine* line, Parser* parser) {
	ScanPos pos(line);
	return scan_line(&pos, parser);
#if 0
	for (;;) {
		const char* YYTOKEN = YYCURSOR;
		/*~~!re2c
			re2c:define:YYCTYPE = char;
			re2c:yyfill:enable = 0;
			re2c:indent:top = 2;



			'NOP' /nw			{ PUSH(TK_NOP); continue; }
			'ORG' /nw			{ PUSH(TK_ORG); continue; }

		*/
		continue;

	include1:
		YYTOKEN = YYCURSOR;
		/*~~!re2c
			*					{ SYNTAX_ERROR(); break; }


		*/
	}

	// push a newline to synchronize parser even on errors
	parser->push(TK_ENDL);
	return ok;
#endif
}

#endif
#endif
