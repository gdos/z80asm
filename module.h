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
#include <vector>

class Module : noncopyable {
public:
	Module(const std::string& name);
	virtual ~Module();

	void add_opcode(Opcode* opcode);

private:
	std::string	name_;					// module name
	Symtab*		symtab_;				// symbols
	std::vector<Section*> sections_;	// list of sections
	Section*	section_;				// point to current section
};

#endif // ndef MODULE_H_
