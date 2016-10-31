//-----------------------------------------------------------------------------
// z80asm - symbols
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "symbol.h"

Symbol::Symbol(SymbolType type, const std::string& name)
    : type_(type), name_(name) {
}

Symbol::~Symbol() {
}

ConstSymbol::ConstSymbol(const std::string& name, int value)
    : Symbol(kConstSymbol, name), value_(value) {
}

ConstSymbol::~ConstSymbol() {
}

LabelSymbol::LabelSymbol(const std::string& name, Instr* wptr_code)
    : Symbol(kLabelSymbol, name), wptr_code_(wptr_code) {
}

LabelSymbol::~LabelSymbol() {
}

ExprSymbol::ExprSymbol(const std::string& name, Expr* wptr_expr)
    : Symbol(kExprSymbol, name), wptr_expr_(wptr_expr) {
}

ExprSymbol::~ExprSymbol() {
}

