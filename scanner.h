//-----------------------------------------------------------------------------
// z80asm scanner
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SCANNER_H_
#define SCANNER_H_

#include "fwd.h"

enum ScanState {
    kScanAtLabel, kScanAtOperation, kScanAtArgs, kScanAtEos
};

class Scanner {
public:
    Scanner(SourceStack* source, ObjectFile* obj);
    virtual ~Scanner();

    bool parse();

private:
    bool parse_line(SourceLine* line);

    SourceStack*    source_;
    ObjectFile*     obj_;
    ScanState       state_;
};

#endif // SCANNER_H_


