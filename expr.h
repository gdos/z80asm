//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef EXPR_H_
#define EXPR_H_

#include "fwd.h"

#include <vector>

enum ExprType {
    kConstExpr, kSymbolExpr, kAryExpr
};

enum OpType {
    kAddOp, kSubtractOp, // ...
};

class Expr {
public:
    Expr(ExprType type);
    virtual ~Expr();

protected:
    ExprType type_;
};

class ConstExpr : public Expr {
public:
    ConstExpr(int value);
    virtual ~ConstExpr();

private:
    int value_;
};

class SymbolExpr : public Expr {
public:
    SymbolExpr(Symbol* wptr_symbol);
    virtual ~SymbolExpr();

private:
    Symbol* wptr_symbol_;
};

class AryExpr : public Expr {
public:
    AryExpr(OpType op_type);
    AryExpr(OpType op_type, Expr* arg1);
    AryExpr(OpType op_type, Expr* arg1, Expr* arg2);
    AryExpr(OpType op_type, Expr* arg1, Expr* arg2, Expr* arg3);
    virtual ~AryExpr();

    void add_arg(Expr* arg);

private:
    OpType op_type_;
    std::vector<Expr*> args_;     // owns the sub-expressions
};

#endif // EXPR_H_
