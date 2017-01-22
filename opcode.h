//-----------------------------------------------------------------------------
// z80asm - assembly operation code
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef OPCODE_H_
#define OPCODE_H_

#include "memcheck.h"
#include "fwd.h"
#include <vector>

class Opcode : noncopyable {
public:
	static const int UNDEFINED_ADDRESS = -1;
	static const int UNDEFINED_CODE = -1;

	// CPU constants
	enum {
		NOP = 0
	};

	// types of opcode
	enum OpcodeType {
		OPC_LABEL, OPC_CODE, OPC_DATA
	};

	Opcode(OpcodeType type, SrcLine* line = NULL, int opcode = UNDEFINED_CODE);
	virtual ~Opcode();

	OpcodeType type() const { return type_; }
	SrcLine* line() { return line_; }
	int opcode() const { return opcode_; }
	int address() const { return address_; }
	void set_address(int address) { address_ = address; }

	// factory methods
	static Opcode* nop(SrcLine* line = NULL);

private:
	OpcodeType	type_;						// type of operation
	SrcLine*	line_;						// source line weak pointer
	int			opcode_;					// numeric opcode of CPU instruction
	int			address_;					// address defined after link stage

	std::vector<byte> bytes_;				// instruction bytes, zero placeholders for patch expressions
	std::vector<PatchExpr*> patch_exprs_;	// patch expressions to write on bytes

	static Opcode* cpu_instr(SrcLine* line = NULL, int opcode = NOP, PatchExpr* arg = NULL, PatchExpr* offset = NULL);
};


#endif // OPCODE_H_
