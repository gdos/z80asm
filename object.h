//-----------------------------------------------------------------------------
// z80asm assembled object module
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef OBJECT_H_
#define OBJECT_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>
#include <vector>

class Object : noncopyable {
public:
	Object();
	virtual ~Object();

	bool assemble(const std::string& source_file);
	bool load(const std::string& object_file);
	bool load_or_assemble(const std::string& file);

	Source* source() { return source_; }

	bool open_source(std::string file, SrcLine* from = NULL, int column = 0);
	bool parse();

private:
	Source*		source_;			// source code
	Parser*		parser_;			// source parser
	std::vector<Module*> modules_;	// list of modules
};

#endif // ndef OBJECT_H_
