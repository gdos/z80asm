//-----------------------------------------------------------------------------
// z80asm
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include <iostream>

#include "preproc.h"
#include "z80asm_config.h"

extern const char* lex(const char* YYCURSOR);

int main() //(int argc, char* argv[])
{
	std::cout << "z80asm version " 
		<< Z80ASM_VERSION_MAJOR << "." << Z80ASM_VERSION_MINOR 
		<< std::endl;
	return 0;
}
