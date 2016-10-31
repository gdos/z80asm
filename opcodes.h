//-----------------------------------------------------------------------------
// z80asm opcodes to store in Instr
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef OPCODES_H_
#define OPCODES_H_

#include <string>

// types of opcodes
enum OpcodeType {
    kOpcodeByte,           // e.g. nop
};

// represent one operation code
class Opcode {
public:
    Opcode(OpcodeType type, unsigned bytes, int tstates, const std::string& text);
    virtual ~Opcode();

    int size() const;

private:
    OpcodeType  type_;
    unsigned    bytes_;         // store opcode bytes in MSB order, e.g. 0xCB01 for 0xCB, 0x01
    int         tstates_;       // T-states
    std::string text_;          // disassembly
};

// declare all opcodes
#define OPCODE(name, type, bytes, tstates, text) \
    extern Opcode name
#include "opcodes.inc"
#undef OPCODE

#endif // OPCODES_H_
