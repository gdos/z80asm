//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "expr.h"
#include "symbol.h"

// TODO: simplify expressions to tranform LABEL1-LABEL2 into a constant

// exponentiation by squaring
Result Expr::power(int base, int exp) {
	int result = 1;
	if (exp < 0) {
		return Result(0);
	}
	else {
		while (exp) {
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		}
		return Result(result);
	}
}

SymbolExpr::SymbolExpr(Symbol* symbol)
	: symbol_(symbol) {
}

SymbolExpr::~SymbolExpr() {
	// do not delete weak poiter symbol_
}

Result SymbolExpr::operator()() const {
	Result result;
	int level = symbol_->reserve();
	if (level == 1) {		// can recurse
		result = (*symbol_)();
	}
	else {
		result = Result(0, Result::RECURSIVE_SYMBOL);
	}
	level = symbol_->release();
	return result;
}

Expr* SymbolExpr::clone() const {
	return new SymbolExpr(symbol_);
}
