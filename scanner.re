//-----------------------------------------------------------------------------
// z80asm scanner
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "scanner.h"
#include "source.h"
#include "icode.h"
#include "opcodes.h"
#include "error.h"
#include <cassert>

Scanner::Scanner(SourceStack* source, ObjectFile* obj)
    : source_(source), obj_(obj), state_(kScanAtLabel) {
}

Scanner::~Scanner() {
}

bool Scanner::parse() {
    bool ok = true;
    SourceLine* line;
    while ((line = source_->getline()) != NULL) {   // read all input          
        if (!parse_line(line))
            ok = false;
    }
    return ok;
}

bool Scanner::parse_line(SourceLine* line) {
    bool ok = true;
    const char* YYCURSOR = line->text.c_str();
	const char* YYMARKER;
    const char* YYCTXMARKER;
	const char* token;
    state_ = kScanAtLabel;

    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;
        ws = [ \t];
        word     =  [_0-9a-zA-Z];
        not_word = [^_0-9a-zA-Z];
    */
    while (ok && *YYCURSOR != '\0') {   // read all tokens
		token = YYCURSOR;
        switch (state_) {
        case kScanAtLabel:

            // fall through
        case kScanAtOperation:
            /*!re2c
				ws+						{ continue; }
                'nop' / not_word		{ obj_->add_opcode(line, &opc_nop); 
                                          state_ = kScanAtEos; 
                                          continue; }
				*						{  goto scan_error; }
            */

            break;

        case kScanAtArgs:

            break;

        case kScanAtEos:

            break;

        default:
            assert(0);
        }

        // input not recognized
		scan_error:
        ok = false;
        error_scan(line, token);
    }
    return ok;
}