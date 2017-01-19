//-----------------------------------------------------------------------------
// z80asm assembled module
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef MODULE_H_
#define MODULE_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>

class Module : noncopyable {
public:
	Module(const std::string& name);
	virtual ~Module();

	bool open(std::string source, SrcLine* from = NULL);
	bool parse();

private:
	std::string	name_;			// module name
	Symtab*		symtab_;		// symbols
};

#endif // ndef MODULE_H_
