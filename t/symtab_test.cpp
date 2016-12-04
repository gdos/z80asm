//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "symtab.h"
#include "symbol.h"
#include "expr.h"
#include "opcode.h"
#include "source.h"
#include "test.h"
#include <iostream>

#define T_EVAL(sym_, value_, error_) \
			r = (*sym_)(); \
			IS(r.value, value_); \
			IS(r.error, error_)
#define T_SYM(sym_, id_, value_, error_, line_, prev_) \
			OK(sym_ != NULL); \
			IS(sym_->id(), id_); \
			T_EVAL(sym_, value_, error_); \
			OK(sym_->line() == line_); \
			OK(sym_->prev() == prev_)

int main() {
	START_TESTING();

	Symtab* tab;
	Symbol *sym, *sym_L1, *sym_L2, *sym_C1, *sym_C2, *sym_V1_1, *sym_V1_2, *sym_V1_3, *sym_V2_1, *sym_V2_2;
	SrcLine line_use, line_L1, line_L2, line_C1, line_C2, line_V1_1, line_V1_2, line_V1_3, line_V2_1, line_V2_2;
	Result r;
	Opcode opcode_L1, opcode_L2;
	opcode_L1.set_address(21);
	opcode_L2.set_address(22);

	tab = new Symtab;
	IS(tab->size(), 0);

	// label - define before use
	sym_L1 = tab->define_label("L1", &opcode_L1, &line_L1);
	IS(tab->size(), 1);
	T_SYM(sym_L1, 1, 21, Result::OK, &line_L1, NULL);

	sym = tab->use_symbol("L1", &line_use);
	IS(tab->size(), 1);
	OK(sym == sym_L1);
	T_SYM(sym, 1, 21, Result::OK, &line_L1, NULL);

	// multiply defined
	sym = tab->define_label("L1", &opcode_L1, &line_L1);
	IS(tab->size(), 1);
	OK(sym == NULL);

	// label - declare before use
	sym_L2 = tab->use_symbol("L2", &line_use);
	IS(tab->size(), 2);
	T_SYM(sym_L2, 2, 0, Result::UNDEFINED_SYMBOL, &line_use, NULL);

	sym = tab->define_label("L2", &opcode_L2, &line_L2);
	OK(sym == sym_L2);
	T_SYM(sym, 2, 22, Result::OK, &line_L2, NULL);

	// constant - define before use
	sym_C1 = tab->define_constant("C1", new NumberExpr(31), &line_C1);
	IS(tab->size(), 3);
	T_SYM(sym_C1, 3, 31, Result::OK, &line_C1, NULL);

	sym = tab->use_symbol("C1", &line_use);
	IS(tab->size(), 3);
	OK(sym == sym_C1);
	T_SYM(sym, 3, 31, Result::OK, &line_C1, NULL);

	// multiply defined
	sym = tab->define_constant("C1", new NumberExpr(31), &line_C1);
	IS(tab->size(), 3);
	OK(sym == NULL);

	// constant - declare before use
	sym_C2 = tab->use_symbol("C2", &line_use);
	IS(tab->size(), 4);
	T_SYM(sym_C2, 4, 0, Result::UNDEFINED_SYMBOL, &line_use, NULL);

	sym = tab->define_constant("C2", new NumberExpr(32), &line_C2);
	OK(sym == sym_C2);
	T_SYM(sym, 4, 32, Result::OK, &line_C2, NULL);

	// variable - define before use
	sym_V1_1 = tab->define_variable("V1", new NumberExpr(51), &line_V1_1);
	IS(tab->size(), 5);
	T_SYM(sym_V1_1, 5, 51, Result::OK, &line_V1_1, NULL);

	sym = tab->use_symbol("V1", &line_use);
	IS(tab->size(), 5);
	OK(sym == sym_V1_1);
	T_SYM(sym, 5, 51, Result::OK, &line_V1_1, NULL);

	// multiply defined
	sym = tab->define_label("V1", &opcode_L1, &line_L1);
	IS(tab->size(), 5);
	OK(sym == NULL);

	sym = tab->define_constant("V1", new NumberExpr(31), &line_C1);
	IS(tab->size(), 5);
	OK(sym == NULL);

	// V1 := V1 + 1
	sym_V1_2 = tab->define_variable("V1", new AddExpr(new SymbolExpr(sym_V1_1), new NumberExpr(1)), &line_V1_2);
	IS(tab->size(), 5);
	T_SYM(sym_V1_2, 6, 52, Result::OK, &line_V1_2, sym_V1_1);

	sym = tab->use_symbol("V1", &line_use);
	IS(tab->size(), 5);
	OK(sym == sym_V1_2);
	T_SYM(sym, 6, 52, Result::OK, &line_V1_2, sym_V1_1);

	// V1 := V1 + 1
	sym_V1_3 = tab->define_variable("V1", new AddExpr(new SymbolExpr(sym_V1_2), new NumberExpr(1)), &line_V1_3);
	IS(tab->size(), 5);
	T_SYM(sym_V1_3, 7, 53, Result::OK, &line_V1_3, sym_V1_2);

	sym = tab->use_symbol("V1", &line_use);
	IS(tab->size(), 5);
	OK(sym == sym_V1_3);
	T_SYM(sym, 7, 53, Result::OK, &line_V1_3, sym_V1_2);

	// variable - declare before use
	sym_V2_1 = tab->use_symbol("V2", &line_use);
	IS(tab->size(), 6);
	T_SYM(sym_V2_1, 8, 0, Result::UNDEFINED_SYMBOL, &line_use, NULL);

	// V2 := V2 + 1
	sym_V2_2 = tab->define_variable("V2", new AddExpr(new SymbolExpr(sym_V2_1), new NumberExpr(1)), &line_V2_2);
	IS(tab->size(), 6);
	T_SYM(sym_V2_2, 9, 1, Result::OK, &line_V2_2, sym_V2_1);

	sym = tab->use_symbol("V2", &line_use);
	IS(tab->size(), 6);
	OK(sym == sym_V2_2);
	T_SYM(sym, 9, 1, Result::OK, &line_V2_2, sym_V2_1);

	// iterate
	for (Symtab::iterator it = tab->begin(); it != tab->end(); ++it) {
		sym = it->second;
		switch (sym->id()) {
		case 1: OK(sym == sym_L1); break;
		case 2: OK(sym == sym_L2); break;
		case 3:	OK(sym == sym_C1); break;
		case 4:	OK(sym == sym_C2); break;
		case 7: OK(sym == sym_V1_3); OK(sym->prev() == sym_V1_2); OK(sym->prev()->prev() == sym_V1_1); break;
		case 9: OK(sym == sym_V2_2); OK(sym->prev() == sym_V2_1); break;
		default: DIAG(sym->id());  FAIL();
		}
	}

#if 0
	test_Symtab(false);
	test_Symtab(true);

	Symtab* tab;
	Symbol* sym;
	Symbol* sym2;
	Result r;
	Opcode opcode;
	opcode.set_address(23);

	tab = new Symtab;

	// use some
	sym = tab->use_symbol("L1");
	IS(sym->type(), TYPE_UNDEFINED);

	sym = tab->use_symbol("C1");
	IS(sym->type(), TYPE_UNDEFINED);

	sym = tab->use_symbol("V1");
	IS(sym->type(), TYPE_UNDEFINED);

	// label
	sym = tab->define_label("L1", &opcode);
	IS(sym->type(), TYPE_LABEL);
	T_SYM_VALUE(23);

	sym = tab->use_symbol("L1");
	IS(sym->type(), TYPE_LABEL);
	T_SYM_VALUE(23);

	sym = tab->define_label("L1", &opcode);
	IS(sym, NULL);

	sym = tab->use_symbol("L1");
	IS(sym->type(), TYPE_LABEL);
	T_SYM_VALUE(23);

	// constant
	sym = tab->define_constant("C1", new NumberExpr(41));
	IS(sym->type(), TYPE_CONSTANT);
	T_SYM_VALUE(41);

	sym = tab->use_symbol("C1");
	IS(sym->type(), TYPE_CONSTANT);
	T_SYM_VALUE(41);

	sym = tab->define_constant("C1", new NumberExpr(45));
	IS(sym, NULL);

	sym = tab->use_symbol("C1");
	IS(sym->type(), TYPE_CONSTANT);
	T_SYM_VALUE(41);

	// variable
	sym = tab->define_variable("V1", new NumberExpr(0));
	IS(sym->type(), TYPE_VARIABLE);
	T_SYM_VALUE(0);

	//sym2=tab->define_variable("V1",new AddExpr(sym->get_value()))

#endif

	delete tab;

	DONE_TESTING();
}
