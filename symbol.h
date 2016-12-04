//-----------------------------------------------------------------------------
// z80asm - symbols
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "memcheck.h"
#include "fwd.h"
#include "result.h"
#include <string>

// is it defined? if yes, what type?
enum SymbolType {
	TYPE_UNDEFINED,		
	TYPE_LABEL,			// label:
	TYPE_CONSTANT,		// DEFC
	TYPE_VARIABLE,		// DEFL
};

// was it declared? if yes, what scope?
enum SymbolDecl {
	DECL_UNDEFINED,
	DECL_GLOBAL,		// extern if not defined, public if defined
	DECL_PUBLIC,		// exported
	DECL_EXTERN,		// imported
	DECL_LOCAL,			// local
};

// Value of a symbol
class SymbolValue : noncopyable {
public:
	SymbolValue();
	virtual ~SymbolValue();
	virtual SymbolValue* clone() const = 0;		// make a copy
	virtual Result operator()() const = 0;		// evaluate
	virtual SymbolType type() const = 0;
};

// undefined value - used or declared but not defined
class UndefinedValue : public SymbolValue {
public:
	UndefinedValue();
	virtual UndefinedValue* clone() const;
	virtual Result operator()() const;
	virtual SymbolType type() const { return TYPE_UNDEFINED; }
};

// label value
class LabelValue : public SymbolValue {
public:
	LabelValue(Opcode* opcode);
	virtual LabelValue* clone() const;
	virtual Result operator()() const;
	virtual SymbolType type() const { return TYPE_LABEL; }

protected:
	Opcode* opcode_;		// weak pointer
};

// constant value
class ConstantValue : public SymbolValue {
public:
	ConstantValue(Expr* expr);
	~ConstantValue();
	virtual ConstantValue* clone() const;
	virtual Result operator()() const;
	virtual SymbolType type() const { return TYPE_CONSTANT; }

protected:
	Expr* expr_;			// strong pointer
};

// variable value
class VariableValue : public ConstantValue {
public:
	VariableValue(Expr* expr);
	virtual VariableValue* clone() const;
	virtual SymbolType type() const { return TYPE_VARIABLE; }
};

// symbol
class Symbol : noncopyable {
public:
	Symbol(const std::string& name, SymbolValue* value,
		SrcLine* line = NULL, Symbol* prev = NULL, int id = 0, SymbolDecl decl = DECL_UNDEFINED);
	virtual ~Symbol();
	Symbol* clone() const;		// make a copy
	Result operator()() const;	// evaluate

	// getters, setters
	const std::string& name() const { return name_; }
	SymbolType type() const { return value_->type(); }

	SymbolValue* value() { return value_; }
	void set_value(SymbolValue* value);

	SrcLine* line() { return line_; }
	void set_line(SrcLine* line) { line_ = line; }

	Symbol* prev() { return prev_; }
	void set_prev(Symbol* prev) { prev_ = prev; }

	int id() const { return id_; }
	void set_id(int id) { id_ = id; }

	SymbolDecl decl() const { return decl_; }
	void set_decl(SymbolDecl decl) { decl_ = decl; }

	// recursion detection in SymbolExpr
	int reserve() { return ++level_; }
	int release() { return --level_; }

protected:
	std::string		name_;
	SymbolValue*	value_;
	SrcLine*		line_;		// weak pointer
	Symbol*			prev_;		// strong pointer, stores previous value of symbol variable
	int				id_;		// symtab id
	SymbolDecl		decl_;
	int				level_;		// detect recursive expressions

	Symbol*			prev_clone() const { return prev_ ? prev_->clone() : NULL; }
};

#endif // SYMBOL_H_
