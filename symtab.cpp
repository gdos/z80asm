//-----------------------------------------------------------------------------
// z80asm - symbol table
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "symtab.h"
#include "symbol.h"
#include "expr.h"
#include <cassert>

Symtab::Symtab()
	: next_id_(0) {
}

Symtab::~Symtab() {
	for (iterator it = symbols_.begin(); it != symbols_.end(); ++it)
		delete it->second;
	symbols_.clear();
}

Symbol* Symtab::find(const std::string& name) {
	iterator it = symbols_.find(name);
	if (it == symbols_.end())
		return NULL;
	else
		return it->second;
}

Symbol* Symtab::extract(const std::string& name) {
	iterator it = symbols_.find(name);
	if (it == symbols_.end()) {
		return NULL;
	}
	else {
		Symbol *symbol = it->second;
		symbols_.erase(it);
		return symbol;
	}
}

Symbol* Symtab::create(const std::string& name, SrcLine* line) {
	Symbol* prev = extract(name);
	int id = ++next_id_;
	Symbol *symbol = new Symbol(name, new UndefinedValue(), line, prev, id);
	symbols_[name] = symbol;
	return symbol;
}

Symbol* Symtab::find_or_create(const std::string& name, SrcLine* line) {
	Symbol* symbol = find(name);
	if (symbol == NULL)
		symbol = create(name, line);
	return symbol;
}

Symbol* Symtab::use_symbol(const std::string& name, SrcLine* line) {
	return find_or_create(name, line);
}

Symbol* Symtab::define_label(const std::string& name, Opcode* opcode, SrcLine* line) {
	Symbol* symbol = find_or_create(name, line);
	if (symbol->type() == TYPE_UNDEFINED) {
		symbol->set_value(new LabelValue(opcode));
		symbol->set_line(line);
		return symbol;
	}
	else {
		return NULL;			// multiply defined
	}
}

Symbol* Symtab::define_constant(const std::string& name, Expr* expr, SrcLine* line) {
	Symbol* symbol = find_or_create(name, line);
	if (symbol->type() == TYPE_UNDEFINED) {
		symbol->set_value(new ConstantValue(expr));
		symbol->set_line(line);
		return symbol;
	}
	else {
		delete expr;
		return NULL;			// multiply defined
	}
}

Symbol* Symtab::define_variable(const std::string& name, Expr* expr, SrcLine* line) {
	Symbol* prev = find(name);
	if (prev != NULL) {
		if (prev->type() == TYPE_UNDEFINED)
			prev->set_value(new VariableValue(new NumberExpr(0)));	// already used and not defined - set initial value

		if (prev->type() != TYPE_VARIABLE) {
			delete expr;
			return NULL;		// multiply defined
		}
	}
	
	Symbol* symbol = create(name, line);
	symbol->set_value(new VariableValue(expr));
	symbol->set_line(line);

	if (prev != NULL)
		symbol->set_decl(prev->decl());
	return symbol;
}
