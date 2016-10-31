//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "expr.h"

Expr::Expr(ExprType type)
    : type_(type) {
}

Expr::~Expr() {
}

ConstExpr::ConstExpr(int value)
    : Expr(kConstExpr), value_(value) {
}

ConstExpr::~ConstExpr() {
}

SymbolExpr::SymbolExpr(Symbol* wptr_symbol)
    : Expr(kSymbolExpr), wptr_symbol_(wptr_symbol) {
}

SymbolExpr::~SymbolExpr() {
}

AryExpr::AryExpr(OpType op_type)
    : Expr(kAryExpr), op_type_(op_type) {
}

AryExpr::AryExpr(OpType op_type, Expr* arg1)
    : Expr(kAryExpr), op_type_(op_type) {
    add_arg(arg1);
}

AryExpr::AryExpr(OpType op_type, Expr* arg1, Expr* arg2)
    : Expr(kAryExpr), op_type_(op_type) {
    add_arg(arg1);
    add_arg(arg2);
}

AryExpr::AryExpr(OpType op_type, Expr* arg1, Expr* arg2, Expr* arg3)
    : Expr(kAryExpr), op_type_(op_type) {
    add_arg(arg1);
    add_arg(arg2);
    add_arg(arg3);
}

AryExpr::~AryExpr() {
    for (std::vector<Expr*>::iterator it = args_.begin(); it != args_.end(); ++it)
        delete *it;
    args_.clear();
}

void AryExpr::add_arg(Expr* arg) {
    args_.push_back(arg);
}
