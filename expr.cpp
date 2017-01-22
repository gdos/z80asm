//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "expr.h"
#include <cassert>

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


PatchExpr::PatchExpr(PatchType type, Expr* expr, int offset)
	: type_(type), expr_(expr), offset_(offset) {}

PatchExpr::~PatchExpr() {
	delete expr_;
}

int PatchExpr::size() const {
	switch (type_) {
	case PATCH_BYTE: return 1;
	case PATCH_WORD: return 2;
	case PATCH_TRI: return 3;
	case PATCH_QUAD: return 4;
	case PATCH_OFFSET: return 1;
	default: assert(0); 
	}
	return 0;	// not reached
}
