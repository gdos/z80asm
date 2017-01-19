//-----------------------------------------------------------------------------
// z80asm assembled object module
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "object.h"
#include "options.h"
#include "message.h"
#include "module.h"
#include "parser.h"
#include "source.h"
#include "util.h"

Object::Object() 
	: source_(new Source())
	, parser_(new Parser(this)) {
	modules_.push_back(new Module(""));
}

Object::~Object() {
	delete source_;
	delete parser_;
	for (std::vector<Module*>::iterator it = modules_.begin(); it != modules_.end(); ++it)
		delete *it;
}

bool Object::assemble(const std::string& source_file) {
	if (opts.verbose())
		msg::assembling(source_file);

	if (!open_source(source_file))
		return false;

	// add source file directory to the end of the include path during parse
	// to allow relative includes
	opts.push_include_path(util::dirname(source_file));
	bool ok = parse();
	opts.pop_include_path();

	return ok;
}

bool Object::load(const std::string& object_file) {
	if (opts.verbose())
		msg::loading(object_file);

	return true; 
}

bool Object::load_or_assemble(const std::string& file) {
	// TODO: load object file if up-to-date
	return assemble(file);
}

bool Object::open_source(std::string file, SrcLine* from, int column) {
	file = opts.search_include_path(file);

	if (source_->has_file(file)) {
		err::recursive_include(file, from, column);
		return false;
	}

	if (!source_->open(file)) {
		err::open_file(file, from, column);
		return false;
	}

	if (opts.verbose())
		msg::reading(file);

	return true;
}

bool Object::parse() {
	return parser_->parse();
}
