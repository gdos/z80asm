//-----------------------------------------------------------------------------
// z80asm assembled module
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "module.h"
#include "message.h"
#include "section.h"
#include "source.h"
#include "options.h"

Module::Module(const std::string& name)
	: name_(name) {
	sections_.push_back(new Section(""));
	section_ = sections_.back();
}

Module::~Module() {
	for (std::vector<Section*>::iterator it = sections_.begin(); it != sections_.end(); ++it)
		delete *it;
	sections_.clear();
}

void Module::add_opcode(Opcode* opcode) {
	section_->add_opcode(opcode);
}
