//-----------------------------------------------------------------------------
// test.h -- test utilities
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef T_TEST_H_
#define T_TEST_H_

#include "taptest.h"

#include <cstdio>
#include <sys/stat.h>

#include <iostream>
#include <fstream>


// create test file
inline void create_test_file(const char* filename, const char* text)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.good()) {
		std::perror(filename);
		FAIL();
	}
	else {
		PASS();
		file << text;
		file.close();
	}
}

// delete test file
inline void delete_test_file(const char* filename)
{
	struct stat status;
	if (stat(filename, &status) == 0) {		// file exists
		if (std::remove(filename) != 0) {	// remove failed
			std::perror(filename);
			FAIL();
		}
	}
}

#endif // T_TEST_H_
