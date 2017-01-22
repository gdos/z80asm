//-----------------------------------------------------------------------------
// z80asm section of code
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "section.h"
#include "opcode.h"

Section::Section(const std::string& name)
	: name_(name) {}

Section::~Section() {
	for (std::vector<Opcode*>::iterator it = opcodes_.begin(); it != opcodes_.end(); ++it)
		delete *it;
	opcodes_.clear();
}

void Section::add_opcode(Opcode* opcode) {
	opcodes_.push_back(opcode);
}
