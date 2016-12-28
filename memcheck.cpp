//-----------------------------------------------------------------------------
// Visual Studio memory checker
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"

#if defined(_MSC_VER) && defined(_DEBUG)

bool MemoryLeaksDumper::inited = false;

#define REPORT_STDERR(reportType)   \
    _CrtSetReportMode(reportType, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG); \
    _CrtSetReportFile(reportType, _CRTDBG_FILE_STDERR )

MemoryLeaksDumper::MemoryLeaksDumper() {
	if (!inited) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		REPORT_STDERR(_CRT_WARN);
		REPORT_STDERR(_CRT_ERROR);
		REPORT_STDERR(_CRT_ASSERT);

		// break on alloc Nr xx
		//_crtBreakAlloc = 351;

		inited = true;
	}
}

#undef REPORT_STDERR

#endif