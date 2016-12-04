//-----------------------------------------------------------------------------
// z80asm - symbol table
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SYMTAB_H_
#define SYMTAB_H_

#include "memcheck.h"
#include "fwd.h"
#include "symbol.h"
#include <string>
#include <vector>
#include <unordered_map>

class Symtab : noncopyable {
public:
	typedef std::unordered_map<std::string, Symbol*> container;
	typedef container::iterator iterator;

	Symtab();
	virtual ~Symtab();

	// container
	iterator begin() { return symbols_.begin(); }
	iterator end() { return symbols_.end(); }
	std::size_t size() { return symbols_.size(); }

	// create/return existing symbol; return NULL on error
	Symbol* use_symbol(const std::string& name, SrcLine* line = NULL);						// used in expression
	Symbol* define_label(const std::string& name, Opcode* opcode, SrcLine* line = NULL);	// label:
	Symbol* define_constant(const std::string& name, Expr* expr, SrcLine* line = NULL);		// DEFC
	Symbol* define_variable(const std::string& name, Expr* expr, SrcLine* line = NULL);		// DEFL

protected:
	container symbols_;
	int next_id_;

	Symbol* find(const std::string& name);
	Symbol* extract(const std::string& name);
	Symbol* create(const std::string& name, SrcLine* line = NULL);
	Symbol* find_or_create(const std::string& name, SrcLine* line = NULL);
};

#endif // SYMTAB_H_
