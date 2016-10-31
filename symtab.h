//-----------------------------------------------------------------------------
// z80asm - symbol table
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SYMTAB_H_
#define SYMTAB_H_

#include "fwd.h"
#include <string>
#include <map>

class SymbolTable {
public:
    SymbolTable();
    virtual ~SymbolTable();

private:
    std::map<std::string, Symbol*> symbols_;    // owns symbols
};

#endif // SYMTAB_H_
