//-----------------------------------------------------------------------------
// z80asm - assembly operation code
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "opcode.h"
#include "expr.h"
#include <cassert>

Opcode::Opcode(OpcodeType type, SrcLine* line, int opcode)
	: type_(type), line_(line), opcode_(opcode), address_(UNDEFINED_ADDRESS) {}

Opcode::~Opcode() {
	bytes_.clear();
	for (std::vector<PatchExpr*>::iterator it = patch_exprs_.begin(); it != patch_exprs_.end(); ++it)
		delete *it;
	patch_exprs_.clear();
}

// factory methods
Opcode* Opcode::cpu_instr(SrcLine* line, int opcode, PatchExpr* arg, PatchExpr* offset) {
	Opcode* op = new Opcode(OPC_CODE, line);

	op->opcode_ = opcode;

	// byte 0
	op->bytes_.push_back(opcode & 0xFF); opcode = opcode >> 8;

	// byte 1
	if (opcode != 0) {
		op->bytes_.push_back(opcode & 0xFF); opcode = opcode >> 8;
	}

	// byte 3
	if (offset != NULL) {
		offset->set_offset(op->bytes_.size());
		op->bytes_.push_back(0);

		op->patch_exprs_.push_back(offset);
	}

	// byte 4
	if (opcode != 0) {
		op->bytes_.push_back(opcode & 0xFF); opcode = opcode >> 8;
	}
	assert(opcode == 0);

	// argument
	if (arg != NULL) {
		arg->set_offset(op->bytes_.size());
		for (int i = 0; i < arg->size(); ++i)
			op->bytes_.push_back(0);

		op->patch_exprs_.push_back(arg);
	}

	return op;
}

Opcode* Opcode::nop(SrcLine* line) {
	return cpu_instr(line, NOP);
}
