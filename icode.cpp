//-----------------------------------------------------------------------------
// z80asm intermediate code - represents output of assembly
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "icode.h"
#include "error.h"
#include "expr.h"
#include "symbol.h"
#include "symtab.h"
#include "filesystem/path.h"

Instr::Instr(SourceLine* wptr_line, Opcode* wptr_opcode)
    : wptr_line_(wptr_line), wptr_opcode_(wptr_opcode), address_(0) {
}

Instr::Instr(SourceLine* wptr_line, Opcode* wptr_opcode, Expr* expr1)
    : wptr_line_(wptr_line), wptr_opcode_(wptr_opcode), address_(0) {
    add_expr(expr1);
}

Instr::Instr(SourceLine* wptr_line, Opcode* wptr_opcode, Expr* expr1, Expr* expr2)
    : wptr_line_(wptr_line), wptr_opcode_(wptr_opcode), address_(0) {
    add_expr(expr1);
    add_expr(expr2);
}

Instr::~Instr() {
    for (std::vector<Expr*>::iterator it = exprs_.begin(); it != exprs_.end(); ++it)
        delete *it;
    exprs_.clear();
}

void Instr::add_expr(Expr* expr) {
    exprs_.push_back(expr);
}

Section::Section(const std::string& name)
    : name_(name) {
}

Section::~Section() {
    for (std::vector<Instr*>::iterator it = code_.begin(); it != code_.end(); ++it)
        delete *it;
    code_.clear();
}

void Section::add_instr(Instr* instr) {
    code_.push_back(instr);
}

Module::Module(const std::string& name)
    : name_(name), symtab_(new SymbolTable()) {
    sections_.push_back(new Section());
    cur_section_ = sections_.back();
}

Module::~Module() {
    delete symtab_;
    for (std::vector<Section*>::iterator it = sections_.begin(); it != sections_.end(); ++it)
        delete *it;
    sections_.clear();
}

void Module::add_instr(Instr* instr) {
    cur_section_->add_instr(instr);
}


ObjectFile::ObjectFile(const std::string& obj_filename) 
    : obj_filename_(obj_filename) {
    filesystem::path file(obj_filename_);
    name_ = file.stem();

    modules_.push_back(new Module(name_));		// add default module
    cur_module_ = modules_.back();  			// point to it
}

ObjectFile::~ObjectFile() {
    for (std::vector<Module*>::iterator it = modules_.begin(); it != modules_.end(); ++it)
        delete *it;
    modules_.clear();
}

void ObjectFile::add_opcode(SourceLine* line, Opcode* opcode) {
    Instr* instr = new Instr(line, opcode);
    cur_module_->add_instr(instr);
}


void ObjectFile::write() {
}