//-----------------------------------------------------------------------------
// Unit tests
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "options.h"
#include "filesystem/path.h"
#include "test.h"

int main()
{
	START_TESTING();
	
	Options opt;
	IS(opt.search_include("path.h"), "path.h");
	opt.add_include_path("filesystem/filesystem");
	IS(filesystem::path(opt.search_include("path.h")), filesystem::path("filesystem/filesystem/path.h"));

	DONE_TESTING();
}
