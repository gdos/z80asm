//-----------------------------------------------------------------------------
// Error output
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef ERROR_H_
#define ERROR_H_

#include "fwd.h"

#include <string>

// help messages output to stdout
extern void cout_version();
extern void cout_usage();

// fatal errors output error message and exit(1)
extern void fatal_open_file(const std::string& filename);   // shows perror()

// parsing errors
extern void error_scan(SourceLine* line, const char* cursor);

#endif // ERROR_H_
