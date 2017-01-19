//-----------------------------------------------------------------------------
// z80asm assembled module
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "module.h"
#include "message.h"
#include "source.h"
#include "options.h"

Module::Module(const std::string& name)
	: name_(name) {}

Module::~Module() {}

bool Module::open(std::string source, SrcLine* from) {
	return true;
}
