//-----------------------------------------------------------------------------
// z80asm section of code
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SECTION_H_
#define SECTION_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>
#include <vector>

class Section : noncopyable {
public:
	Section(const std::string& name);
	virtual ~Section();

	void add_opcode(Opcode* opcode);

private:
	std::string	name_;				// section name
	std::vector<Opcode*> opcodes_;	// opcodes of this section
};

#endif // ndef SECTION_H_
