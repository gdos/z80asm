//-----------------------------------------------------------------------------
// z80asm assembler controller class
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>
#include <vector>

class Assembler : noncopyable {
public:
	Assembler();
	virtual ~Assembler();

	void clear();
	bool assemble(const std::string& source);	// assemble, collect object
	bool good() const { return good_; }

private:
	typedef std::vector<Object*>::iterator iterator;

	std::vector<Object*> objects_;
	bool good_;						// true if all modules compiled OK
};

#endif // ndef ASSEMBLER_H_
