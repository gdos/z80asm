//-----------------------------------------------------------------------------
// z80asm - result of evaluating an expression
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef RESULT_H_
#define RESULT_H_

#include "memcheck.h"
#include "fwd.h"

struct Result {
	// error masks
	static const unsigned OK = 0;
	static const unsigned DIVIDE_ZERO = 1;			// division or modulo by zero
	static const unsigned UNDEFINED_SYMBOL = 2;		// used symbol not defined
	static const unsigned EXTERNAL_SYMBOL = 4;		// depends on external symbol
	static const unsigned UNDEFINED_ADDRESS = 8;	// not linked yet
	static const unsigned RECURSIVE_SYMBOL = 16;	// symbol refers back to itself

	// data
	int			value;
	unsigned	error;	// bit-mask

	// constructor
	Result(int value = 0, unsigned error = OK)
		: value(value), error(error) {}
};


#endif // RESULT_H_
