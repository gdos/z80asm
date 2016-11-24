//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "expr.h"
#include "test.h"
#include <iostream>

#define NUM(x)			new NumberExpr(x)
#define T_OK(VALUE)		r = (*e)(); IS(r.value, VALUE); IS(r.error, Result::OK)
#define T_ERR(ERR)		r = (*e)(); IS(r.value, 0); IS(r.error, ERR)
#define T_FUNC(CLASS,A,B,RES)	e = new CLASS(NUM(A), NUM(B)); T_OK(RES); delete e; \
								e = new CLASS(new DivideExpr(NUM(A), NUM(0)), NUM(B)); T_ERR(Result::DIVIDE_ZERO); delete e; \
								e = new CLASS(NUM(A), new DivideExpr(NUM(B), NUM(0)), NUM(B)); T_ERR(Result::DIVIDE_ZERO); delete e; \
								e = new CLASS(new DivideExpr(NUM(A), NUM(0)), new DivideExpr(NUM(B), NUM(0)), NUM(B)); T_ERR(Result::DIVIDE_ZERO); delete e

int main() {
	START_TESTING();
	Expr* e;
	Result r;

	e = NUM(4);
	T_OK(4);
	delete e;

	e = new AddExpr(new MultiplyExpr(NUM(3), NUM(4)), new MultiplyExpr(NUM(2), NUM(5)));
	T_OK(22);
	delete e;

	e = new ConditionExpr(NUM(1), NUM(42), NUM(666));
	T_OK(42);
	delete e;

	e = new ConditionExpr(NUM(0), NUM(42), NUM(666));
	T_OK(666);
	delete e;

	e = new ConditionExpr(new DivideExpr(NUM(1),NUM(0)), NUM(42), NUM(666));
	T_ERR(Result::DIVIDE_ZERO);
	delete e;

	e = new ConditionExpr(NUM(1), NUM(42), new DivideExpr(NUM(666), NUM(0)));
	T_ERR(Result::DIVIDE_ZERO);
	delete e;

	T_FUNC(LogicalOrExpr, 0, 0, 0);
	T_FUNC(LogicalOrExpr, 0, 12, 1);
	T_FUNC(LogicalOrExpr, 42, 0, 1);
	T_FUNC(LogicalOrExpr, 42, 42, 1);

	T_FUNC(LogicalAndExpr, 0, 0, 0);
	T_FUNC(LogicalAndExpr, 0, 12, 0);
	T_FUNC(LogicalAndExpr, 42, 0, 0);
	T_FUNC(LogicalAndExpr, 42, 42, 1);

	T_FUNC(BinaryOrExpr, 0, 0, 0);
	T_FUNC(BinaryOrExpr, 1, 2, 3);
	T_FUNC(BinaryOrExpr, 0, 4, 4);
	T_FUNC(BinaryOrExpr, 8, 0, 8);

	T_FUNC(BinaryXorExpr, 0, 0, 0);
	T_FUNC(BinaryXorExpr, 1, 0, 1);
	T_FUNC(BinaryXorExpr, 0, 1, 1);
	T_FUNC(BinaryXorExpr, 1, 1, 0);
	T_FUNC(BinaryXorExpr, 8, 12, 4);

	T_FUNC(BinaryAndExpr, 0, 0, 0);
	T_FUNC(BinaryAndExpr, 1, 0, 0);
	T_FUNC(BinaryAndExpr, 0, 1, 0);
	T_FUNC(BinaryAndExpr, 1, 1, 1);
	T_FUNC(BinaryAndExpr, 8, 12, 8);

	T_FUNC(EqualExpr, 6, 5, 0);
	T_FUNC(EqualExpr, 5, 5, 1);
	T_FUNC(EqualExpr, 4, 5, 0);

	T_FUNC(DifferentExpr, 6, 5, 1);
	T_FUNC(DifferentExpr, 5, 5, 0);
	T_FUNC(DifferentExpr, 4, 5, 1);

	T_FUNC(LessExpr, 6, 5, 0);
	T_FUNC(LessExpr, 5, 5, 0);
	T_FUNC(LessExpr, 4, 5, 1);

	T_FUNC(LessEqualExpr, 6, 5, 0);
	T_FUNC(LessEqualExpr, 5, 5, 1);
	T_FUNC(LessEqualExpr, 4, 5, 1);

	T_FUNC(GreaterExpr, 6, 5, 1);
	T_FUNC(GreaterExpr, 5, 5, 0);
	T_FUNC(GreaterExpr, 4, 5, 0);

	T_FUNC(GreaterEqualExpr, 6, 5, 1);
	T_FUNC(GreaterEqualExpr, 5, 5, 1);
	T_FUNC(GreaterEqualExpr, 4, 5, 0);

	T_FUNC(LeftShiftExpr, 1, 0, 1);
	T_FUNC(LeftShiftExpr, 1, 1, 2);
	T_FUNC(LeftShiftExpr, 1, 2, 4);

	T_FUNC(RightShiftExpr, 1, 0, 1);
	T_FUNC(RightShiftExpr, 2, 1, 1);
	T_FUNC(RightShiftExpr, 4, 2, 1);

	T_FUNC(AddExpr, 3, 4, 7);
	T_FUNC(SubtractExpr, 3, 4, -1);
	T_FUNC(MultiplyExpr, 3, 4, 12);

	T_FUNC(DivideExpr, 12, 4, 3);
	T_FUNC(DivideExpr, 11, 4, 2);

	T_FUNC(ModuloExpr, 12, 4, 0);
	T_FUNC(ModuloExpr, 11, 4, 3);

	T_FUNC(PowerExpr, 3, 3, 27);

	T_FUNC(ModuloExpr, 11, 4, 3);

	e = new NegateExpr(NUM(21)); T_OK(-21); delete e;
	e = new NegateExpr(NUM(-21)); T_OK(21); delete e;
	e = new NegateExpr(NUM(0)); T_OK(0); delete e;

	e = new LogicalNotExpr(NUM(0)); T_OK(1); delete e;
	e = new LogicalNotExpr(NUM(1)); T_OK(0); delete e;
	e = new LogicalNotExpr(NUM(33)); T_OK(0); delete e;

	e = new BinaryAndExpr(new BinaryNotExpr(NUM(0x55)), NUM(0xFF)); T_OK(0xAA); delete e;

	DONE_TESTING();
}
