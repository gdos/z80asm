//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef EXPR_H_
#define EXPR_H_

#include "fwd.h"
#include "result.h"
#include <cassert>
#include <cstdlib>

// base expression class
class Expr : noncopyable {
public:
	Expr() {}
	virtual ~Expr() {}
	virtual Result operator()() const = 0;	// evaluate

	// integer power function
	static Result power(int base, int exp);
};

// number leaf
class NumberExpr : public Expr {
public:
	NumberExpr(int value = 0) : value_(value) {}
	Result operator()() const { return Result(value_); }
protected:
	int value_;
};

// N-ary function, N=1..3
template<int NR>
class NaryExpr : public Expr {
public:
	NaryExpr(Expr* a0 = NULL, Expr* a1= NULL, Expr* a2 = NULL) {
		switch (NR) {
		case 3: assert(a2); args_[2] = a2; // fall through
		case 2: assert(a1); args_[1] = a1; // fall through
		case 1: assert(a0); args_[0] = a0; break;
		default:
			assert(0);
		}
	}

	virtual ~NaryExpr() {
		for (int i = 0; i < NR; i++)
			delete args_[i];
	}

	Result operator()() const { 
		int args[NR];
		Result result;
		unsigned error = Result::OK;

		// compute sub-expressions
		for (int i = 0; i < NR; i++) {
			result = (*args_[i])();
			args[i] = result.value;
			error = error | result.error;
		}

		// compute expression
		result = compute(args);
		error = error | result.error;

		if (error != Result::OK)
			return Result(0, error);
		else
			return result;
	}

protected:
	Expr *args_[NR];

private:
	virtual Result compute(int args[]) const = 0;
};


#define DEFINE_CLASS(NAME, NR, EXPR) \
	class NAME : public NaryExpr<NR> { \
	public: NAME(Expr* a0 = NULL, Expr* a1= NULL, Expr* a2 = NULL) \
			: NaryExpr<NR>(a0, a1, a2) {} \
	private: Result compute(int args[]) const { return EXPR; } \
	}

DEFINE_CLASS(ConditionExpr,		3, Result(args[0] ? args[1] : args[2]));
DEFINE_CLASS(LogicalOrExpr,		2, Result(args[0] || args[1]));
DEFINE_CLASS(LogicalAndExpr,	2, Result(args[0] && args[1]));
DEFINE_CLASS(BinaryOrExpr,		2, Result(args[0] | args[1]));
DEFINE_CLASS(BinaryXorExpr,		2, Result(args[0] ^ args[1]));
DEFINE_CLASS(BinaryAndExpr,		2, Result(args[0] & args[1]));
DEFINE_CLASS(EqualExpr,			2, Result(args[0] == args[1]));
DEFINE_CLASS(DifferentExpr,		2, Result(args[0] != args[1]));
DEFINE_CLASS(LessExpr,			2, Result(args[0] < args[1]));
DEFINE_CLASS(LessEqualExpr,		2, Result(args[0] <= args[1]));
DEFINE_CLASS(GreaterExpr,		2, Result(args[0] > args[1]));
DEFINE_CLASS(GreaterEqualExpr,	2, Result(args[0] >= args[1]));
DEFINE_CLASS(LeftShiftExpr,		2, Result(args[0] << args[1]));
DEFINE_CLASS(RightShiftExpr,	2, Result(args[0] >> args[1]));
DEFINE_CLASS(AddExpr,			2, Result(args[0] + args[1]));
DEFINE_CLASS(SubtractExpr,		2, Result(args[0] - args[1]));
DEFINE_CLASS(MultiplyExpr,		2, Result(args[0] * args[1]));
DEFINE_CLASS(DivideExpr,		2, args[1] == 0 ? Result(0, Result::DIVIDE_ZERO) : Result(args[0] / args[1]));
DEFINE_CLASS(ModuloExpr,		2, args[1] == 0 ? Result(0, Result::DIVIDE_ZERO) : Result(args[0] % args[1]));
DEFINE_CLASS(PowerExpr,			2, Expr::power(args[0], args[1]));
DEFINE_CLASS(NegateExpr,		1, Result(- args[0]));
DEFINE_CLASS(LogicalNotExpr,	1, Result(! args[0]));
DEFINE_CLASS(BinaryNotExpr,		1, Result(~ args[0]));

#undef DEFINE_CLASS


#if 0
class SymbolExpr : public Expr {
public:
    SymbolExpr(Symbol* wptr_symbol);
    virtual ~SymbolExpr();

private:
    Symbol* wptr_symbol_;
};
#endif

#endif // EXPR_H_
