//-----------------------------------------------------------------------------
// Test linking of filesystem/path
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include <sys/stat.h>

#include "test.h"
#include "filesystem/path.h"

int main(int argc, char *argv[])
{
	if (argc != 1) {
		FAIL();
	}
	else {
		struct stat status;
		if (stat(argv[0], &status) != 0) {
			FAIL();
		}
		else {
			filesystem::path p(argv[0]);
			DIAG(p << " is " << p.file_size() << " bytes");
			IS(p.file_size(), status.st_size);
			OK(p.exists());
			OK(p.is_regular_file());
			NOK(p.is_directory());
			PASS();
		}
	}

	DONE_TESTING();
}
