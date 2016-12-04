//-----------------------------------------------------------------------------
// z80asm - symbols
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "symbol.h"
#include "opcode.h"
#include "expr.h"
#include "source.h"
#include <cassert>

//-----------------------------------------------------------------------------
// Symbol Value
//-----------------------------------------------------------------------------

// parent class
SymbolValue::SymbolValue() {
}

SymbolValue::~SymbolValue() {
}

// undefined symbol
UndefinedValue::UndefinedValue() {
}

UndefinedValue* UndefinedValue::clone() const {
	return new UndefinedValue();
}

Result UndefinedValue::operator()() const {
	return Result(0, Result::UNDEFINED_SYMBOL);
}

// label
LabelValue::LabelValue(Opcode* opcode)
	: opcode_(opcode) {
}

LabelValue* LabelValue::clone() const {
	return new LabelValue(opcode_);
}

Result LabelValue::operator()() const {
	return opcode_->address();
}

// constant
ConstantValue::ConstantValue(Expr* expr)
	: expr_(expr) {
}

ConstantValue::~ConstantValue() {
	delete expr_;
}

ConstantValue* ConstantValue::clone() const {
	return new ConstantValue(expr_->clone());
}

Result ConstantValue::operator()() const {
	return (*expr_)();
}

// variable
VariableValue::VariableValue(Expr* expr)
	: ConstantValue(expr) {
}

VariableValue* VariableValue::clone() const {
	return new VariableValue(expr_->clone());
}

//-----------------------------------------------------------------------------
// Symbol
//-----------------------------------------------------------------------------
Symbol::Symbol(const std::string& name, SymbolValue* value,
	SrcLine* line, Symbol* prev, int id, SymbolDecl decl)
	: name_(name), value_(value), line_(line), prev_(prev), id_(id), decl_(DECL_UNDEFINED), level_(0) {
}

Symbol::~Symbol() {
	delete prev_;
	delete value_;
}

Symbol* Symbol::clone() const {
	return new Symbol(name_, value_->clone(), line_, prev_clone(), id_, decl_);
}

Result Symbol::operator()() const {
	return (*value_)();
}

void Symbol::set_value(SymbolValue* value) {
	delete value_;
	value_ = value;
}

