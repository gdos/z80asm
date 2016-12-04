//-----------------------------------------------------------------------------
// Visual Studio memory checker
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef MEMCHECK_H_
#define MEMCHECK_H_

#if defined(_MSC_VER) && defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>			// include stdlib.h before crtdbg.h
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

class MemoryLeaksDumper {
public:
	MemoryLeaksDumper();

private:
	static bool inited;
};

static MemoryLeaksDumper memory_leaks_dumper;

#endif // _MSC_VER && _DEBUG
#endif // MEMCHECK_H_
