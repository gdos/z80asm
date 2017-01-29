//-----------------------------------------------------------------------------
// z80asm parser
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "parser.h"
#include "message.h"
#include "object.h"
#include "opcode.h"
#include "options.h"
#include "scanner.h"
#include "source.h"
#include "token.h"
#include <cstdlib>

Parser::Parser(Object* object)
	: object_(object) {
	lemon_ = ParseAlloc(malloc);
}

Parser::~Parser() {
	// do not delete weak pointer object_
	ParseFree(lemon_, free);
}

bool Parser::parse() {
	Scanner scan;

	bool ok = true;
#if 0
	while ((line_ = object_->source()->getline())) {
		scan.init(line_);

		for (bool has_chars = true; has_chars;) {
			switch (scan.get_opcode()) {
			case TK_EOI:
				has_chars = false;
				break;

			case TK_INCLUDE:
				if (!parse_include(&scan)) {
					ok = false;
					scan.flush();
				}
				break;

			case TK_OPCODE_VOID:
				if (!parse_opcode_void(&scan)) {
					ok = false;
					scan.flush();
				}
				break;

			default:
				ok = false;
				scan.error(err::syntax);
			}
		}

		// line scanned, no longer needed for error messages, can be deleted
		if (!opts.do_list())
			line_->clear_text();
	}
#endif

	return ok;
}

bool Parser::parse_include(Scanner* scan) {
#if 0
	if (!scan->get_filename()) 
		return false;
	
	// open file and push to stack of open files, may show error pointing just after file name
//	if (!object_->open_source(scan->text(), scan->from(), scan->column()))
		return false;

	// check for end of statement
	if (!scan->get_end_statement())
		return false;

	// recurse to parse included file and continue in next statement
	SrcLine* save_line = line_;
	bool ok = parse();
	line_ = save_line;

	return ok;
#endif
	return false;
}

bool Parser::parse_opcode_void(Scanner* scan) {
#if 0
	Opcode* opcode;// = scan->opcode();

	// check for end of statement
	if (!scan->get_end_statement()) {
		delete opcode;
		return false;
	}
	else {
		object_->add_opcode(opcode);
		return true;
	}
#endif
	return false;
}
