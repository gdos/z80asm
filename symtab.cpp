//-----------------------------------------------------------------------------
// z80asm - symbol table
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "symtab.h"
#include "symbol.h"

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
    for (std::map<std::string, Symbol*>::iterator it = symbols_.begin(); it != symbols_.end(); ++it)
        delete it->second;
    symbols_.clear();
}
