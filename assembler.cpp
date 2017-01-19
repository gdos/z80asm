//-----------------------------------------------------------------------------
// z80asm assembler worker class
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "assembler.h"
#include "object.h"
#include "options.h"
#include "message.h"
#include "util.h"

Assembler::Assembler() : good_(true) {}

Assembler::~Assembler() {
	clear();
}

void Assembler::clear() {
	for (iterator it = objects_.begin(); it != objects_.end(); ++it)
		delete *it;
	objects_.clear();
	good_ = true;
}

bool Assembler::assemble(const std::string& source) {
	Object* object = new Object();
	
	if (object->load_or_assemble(source)) {
		objects_.push_back(object);
		return true;
	}
	else {
		delete object;
		good_ = false;
		return false;
	}
}
