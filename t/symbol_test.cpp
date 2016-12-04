//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "symbol.h"
#include "expr.h"
#include "opcode.h"
#include "test.h"
#include "source.h"
#include <iostream>

#define T_VALUE(sv_, type_, value_, error_) \
	IS(sv_->type(), type_); \
	r = (*sv_)(); \
	IS(r.value, value_); \
	IS(r.error, error_)

void test_SymbolValue() {
	SymbolValue* sv;
	SymbolValue* sv1;
	Result r;
	Opcode opcode;

	sv = new UndefinedValue();
	T_VALUE(sv, TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL);
	sv1 = sv->clone();
	delete sv;
	T_VALUE(sv1, TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL);
	delete sv1;

	sv = new LabelValue(&opcode);
	T_VALUE(sv, TYPE_LABEL, 0, Result::UNDEFINED_ADDRESS);
	sv1 = sv->clone();
	delete sv;
	T_VALUE(sv1, TYPE_LABEL, 0, Result::UNDEFINED_ADDRESS);
	delete sv1;

	opcode.set_address(42);
	sv = new LabelValue(&opcode);
	T_VALUE(sv, TYPE_LABEL, 42, Result::OK);
	sv1 = sv->clone();
	delete sv;
	T_VALUE(sv1, TYPE_LABEL, 42, Result::OK);
	delete sv1;

	sv = new ConstantValue(new NumberExpr(21));
	T_VALUE(sv, TYPE_CONSTANT, 21, Result::OK);
	sv1 = sv->clone();
	delete sv;
	T_VALUE(sv1, TYPE_CONSTANT, 21, Result::OK);
	delete sv1;

	sv = new VariableValue(new NumberExpr(22));
	T_VALUE(sv, TYPE_VARIABLE, 22, Result::OK);
	sv1 = sv->clone();
	delete sv;
	T_VALUE(sv1, TYPE_VARIABLE, 22, Result::OK);
	delete sv1;
}
#undef T_VALUE

#define T_SYM(sym_, name_, type_, value_, error_, line_, prev_, id_, decl_) \
	IS(sym_->name(), name_); \
	IS(sym_->type(), type_); \
	r = (*sym_)(); \
	IS(r.value, value_); \
	IS(r.error, error_); \
	OK(sym_->line() == line_); \
	OK(sym_->prev() == prev_); \
	IS(sym_->decl(), decl_)

void test_Symbol() {
	Symbol* sym;
	Symbol* sym1;
	Symbol* prev;
	Result r;
	SrcLine line;

	sym = new Symbol("a1", new UndefinedValue());
	T_SYM(sym, "a1", TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL, NULL, NULL, 0, DECL_UNDEFINED);
	sym->set_value(new ConstantValue(new NumberExpr(21)));
	T_SYM(sym, "a1", TYPE_CONSTANT, 21, Result::OK, NULL, NULL, 0, DECL_UNDEFINED);
	sym->set_line(&line);
	T_SYM(sym, "a1", TYPE_CONSTANT, 21, Result::OK, &line, NULL, 0, DECL_UNDEFINED);
	sym->set_prev(new Symbol("a2", new VariableValue(new NumberExpr(33))));
	T_SYM(sym->prev(), "a2", TYPE_VARIABLE, 33, Result::OK, NULL, NULL, 0, DECL_UNDEFINED);
	sym->set_id(3);
	T_SYM(sym, "a1", TYPE_CONSTANT, 21, Result::OK, &line, sym->prev(), 3, DECL_UNDEFINED);
	sym->set_decl(DECL_PUBLIC);
	T_SYM(sym, "a1", TYPE_CONSTANT, 21, Result::OK, &line, sym->prev(), 3, DECL_PUBLIC);
	IS(sym->reserve(), 1);
	IS(sym->reserve(), 2);
	IS(sym->reserve(), 3);
	IS(sym->release(), 2);
	IS(sym->release(), 1);
	IS(sym->release(), 0);
	delete sym;
	
	sym = new Symbol("a1", new UndefinedValue());
	T_SYM(sym, "a1", TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL, NULL, NULL, 0, DECL_UNDEFINED);
	sym1 = sym->clone();
	delete sym;
	T_SYM(sym1, "a1", TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL, NULL, NULL, 0, DECL_UNDEFINED);
	delete sym1;

	sym = new Symbol("a1", new ConstantValue(new NumberExpr(21)));
	T_SYM(sym, "a1", TYPE_CONSTANT, 21, Result::OK, NULL, NULL, 0, DECL_UNDEFINED);
	sym1 = sym->clone();
	delete sym;
	T_SYM(sym1, "a1", TYPE_CONSTANT, 21, Result::OK, NULL, NULL, 0, DECL_UNDEFINED);
	delete sym1;

	prev = new Symbol("a1", new UndefinedValue());
	T_SYM(prev, "a1", TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL, NULL, NULL, 0, DECL_UNDEFINED);
	sym = new Symbol("a1", new ConstantValue(new NumberExpr(21)), &line, prev);
	T_SYM(sym, "a1", TYPE_CONSTANT, 21, Result::OK, &line, prev, 0, DECL_UNDEFINED);
	sym1 = sym->clone();
	delete sym;
	T_SYM(sym1, "a1", TYPE_CONSTANT, 21, Result::OK, &line, sym1->prev(), 0, DECL_UNDEFINED);
	T_SYM(sym1->prev(), "a1", TYPE_UNDEFINED, 0, Result::UNDEFINED_SYMBOL, NULL, NULL, 0, DECL_UNDEFINED);
	delete sym1;
}

int main() {
	START_TESTING();

	test_SymbolValue();
	test_Symbol();

	DONE_TESTING();
}
