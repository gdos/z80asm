//-----------------------------------------------------------------------------
// z80asm opcodes to store in Instr
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "opcodes.h"

#include <cassert>

Opcode::Opcode(OpcodeType type, unsigned bytes, int tstates, const std::string& text)
    : type_(type), bytes_(bytes), tstates_(tstates), text_(text) {
}

Opcode::~Opcode() {
}

int Opcode::size() const {
    switch (type_) {
    case kOpcodeByte: 
        return 1;
    default: 
        assert(0);
        return 0;       // not reached
    }
}

// define all opcodes
#define OPCODE(name, type, bytes, tstates, text) \
    Opcode name(type, bytes, tstates, text)
#include "opcodes.inc"
#undef OPCODE
