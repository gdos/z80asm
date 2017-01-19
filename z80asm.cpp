//-----------------------------------------------------------------------------
// z80asm
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "assembler.h"
#include "options.h"
#include <iostream>

int main(int argc, char* argv[]) {

	if (!opts.parse(argc, argv))
		return 1;			// parse error

	Assembler as;
	for (Options::iterator it = opts.begin_files(); it != opts.end_files(); ++it) {
		as.assemble(*it);
	}
	if (as.good())
		return 0;			// assemble OK
	else
		return 1;			// assemble error
}
