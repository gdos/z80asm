//-----------------------------------------------------------------------------
// z80asm - model expressions
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef EXPR_H_
#define EXPR_H_

#include "memcheck.h"
#include "fwd.h"
#include "result.h"

// base expression class
class Expr : noncopyable {
public:
	Expr() {}
	virtual ~Expr() {}
	virtual Result operator()() const = 0;	// evaluate
	virtual Expr* clone() const = 0;		// make a copy

	static Result power(int base, int exp);	// integer power function
};

// number leaf
class NumberExpr : public Expr {
public:
	NumberExpr(int value = 0) : value_(value) {}
	virtual Result operator()() const { return Result(value_); }
	virtual NumberExpr* clone() const { return new NumberExpr(value_); }

protected:
	int value_;
};


// patch expression
class PatchExpr : noncopyable {
public:
	enum PatchType {
		PATCH_BYTE, PATCH_WORD, PATCH_TRI, PATCH_QUAD, PATCH_OFFSET
	};

	PatchExpr(PatchType type, Expr* expr, int offset = 0);
	virtual ~PatchExpr();

	PatchType type() const { return type_; }
	Expr* expr() { return expr_; }
	int offset() const { return offset_; }
	void set_offset(int offset) { offset_ = offset; }

	int size() const;

private:
	PatchType	type_;			// type of patch
	Expr*		expr_;			// owns expression
	int			offset_;		// offset to patch at
};

#endif // EXPR_H_
