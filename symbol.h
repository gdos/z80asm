//-----------------------------------------------------------------------------
// z80asm - symbols
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "fwd.h"

#include <string>

enum SymbolType {
    kConstSymbol, kLabelSymbol, kExprSymbol
};

class Symbol {
public:
    Symbol(SymbolType type, const std::string& name);
    virtual ~Symbol();

    const std::string& name() const { return name_; }

protected:
    SymbolType  type_;
    std::string name_;
};

class ConstSymbol : public Symbol {
public:
    ConstSymbol(const std::string& name, int value);
    virtual ~ConstSymbol();

private:
    int value_;
};

class LabelSymbol : public Symbol {
    LabelSymbol(const std::string& name, Instr* wptr_code);
    virtual ~LabelSymbol();

private:
    Instr* wptr_code_;
};

class ExprSymbol : public Symbol {
    ExprSymbol(const std::string& name, Expr* wptr_expr);
    virtual ~ExprSymbol();

private:
    Expr* wptr_expr_;
};

#endif // SYMBOL_H_
