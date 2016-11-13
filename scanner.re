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

// state transitions
#define NEXT()	continue
#define GOTO(x)	state_ = (x); NEXT()
#define ERROR()	goto scan_error
#define EMIT(x)	obj_->add_opcode(line, (x))

bool Scanner::parse_line(SourceLine* line) {
    bool ok = true;
    const char* YYCURSOR = line->text.c_str();
	const char* YYMARKER;
    const char* YYCTXMARKER;
	const char* token;
    state_ = kScanAtLabel;
	int r1;
	
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;
        _s_ =  [ \t];
        _S_ = [^ \t];
        _w_ =  [_0-9a-zA-Z];
        _W_ = [^_0-9a-zA-Z];
    */
    while (ok && *YYCURSOR != '\0') {   // read all tokens
		token = YYCURSOR;
        switch (state_) {
        case kScanAtLabel:

            // fall through
        case kScanAtOperation:
            /*!re2c
				_s_+			{ NEXT(); }
				'ld'	/ _W_	{ GOTO(kScan_LD); }
                'nop'	/ _W_	{ EMIT(opc_nop()); GOTO(kScanAtEos); }
				*				{ ERROR(); }
            */
            break;

		case kScan_LD:
            /*!re2c
				_s_+			{ NEXT(); }
				'bc'	/ _W_	{ r1 = 0; GOTO(kScan_LD_dd); }
				'de'	/ _W_	{ r1 = 1; GOTO(kScan_LD_dd); }
				'hl'	/ _W_	{ r1 = 2; GOTO(kScan_LD_dd); }
				'sp'	/ _W_	{ r1 = 3; GOTO(kScan_LD_dd); }
				*				{ ERROR(); }
            */
            break;
		
		case kScan_LD_dd:
            /*!re2c
				_s_+			{ NEXT(); }
				','				{ GOTO(kScan_LD_dd_comma); }
				*				{ ERROR(); }
            */
            break;
			
		case kScan_LD_dd_comma:
            /*!re2c
				*				{ ERROR(); }
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