//-----------------------------------------------------------------------------
// z80asm intermediate code - represents output of assembly
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef ICODE_H_
#define ICODE_H_

#include "fwd.h"

#include <string>

#include "opcodes.h"
#include "source.h"

// represent one instruction
class Instr {
public:
    Instr(SourceLine* wptr_line, const Opcode* wptr_opcode);
    Instr(SourceLine* wptr_line, const Opcode* wptr_opcode, Expr* expr1);
    Instr(SourceLine* wptr_line, const Opcode* wptr_opcode, Expr* expr1, Expr* expr2);
    virtual ~Instr();

    void add_expr(Expr* expr);

private:
    SourceLine*	wptr_line_;
    const Opcode*		wptr_opcode_;
    int					address_;		// address after link step
    std::vector<Expr*>	exprs_;			// owns expressions
};

// represent one section of code
class Section {
public:
    Section(const std::string& name = "");
    virtual ~Section();

    void add_instr(Instr* instr);

private:
    std::string			name_;
    std::vector<Instr*>	code_;      // owns instructions
};

// represent one object module
class Module {
public:
    Module(const std::string& name = "");
    virtual ~Module();

    void add_instr(Instr* instr);

private:
    std::string			name_;
    SymbolTable*        symtab_;
    std::vector<Section*> sections_;    // owns sections
    Section* cur_section_;	            // point to current section
};

// represent one compilation unit
class ObjectFile {
public:
    ObjectFile(const std::string& obj_filename);
    virtual ~ObjectFile();

    void add_opcode(SourceLine* line, const Opcode* opcode);
    void write();                       // write object file

private:
    std::string         obj_filename_;
    std::string			name_;			// main module name = basename of main input file
    std::vector<Module*> modules_;      // owns modules
    Module*             cur_module_;	// point to current module
};

#endif // ICODE_H_
