//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

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
