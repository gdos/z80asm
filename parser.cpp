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
	while ((line_ = object_->source()->getline())) {
		scan.init(line_);

		while (!scan.eoi()) {
			if (!parse_statement(&scan)) {
				ok = false;
				break;			// skip rest of line
			}
		}
	}
	return ok;
}

bool Parser::parse_statement(Scanner* scan) {
	Token* t0 = scan->peek(0);
	switch (t0->id()) {
	case TK_EOI: 
		return true;

	case TK_ENDL:
		scan->next();
		return true;

	case TK_IDENT:
		switch (t0->value()) {
		case TK_INCLUDE:
			return parse_include(scan);

		default:
			goto exit_error;
		}
	default: 
		goto exit_error;
	}

exit_error:
	scan->error(err::syntax);
	return false;
}

bool Parser::parse_include(Scanner* scan) {
	if (!scan->scan_filename())
		return false;

	// open file and push to stack of open files, may show error pointing just after file name
	if (!object_->open_source(scan->text(), scan->line(), scan->column()))
		return false;

	// check for end of statement
	if (!scan->scan_eos())
		return false;

	// recurse to parse included file and continue in next statement
	SrcLine* save_line = line_;
	bool ok = parse();
	line_ = save_line;

	return ok;
}

#if 0

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

	return ok;
}
#endif

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
