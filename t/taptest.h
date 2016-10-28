//-----------------------------------------------------------------------------
// taptest.h -- Macros to implement TAP tests
// NOTE: defines global variables, must be included only in main module
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef T_TAPTEST_H_
#define T_TAPTEST_H_

#include <iostream>

int failed_nr = 0;
int test_nr = 0;

#define DIAG(out) std::cout << "# " << out << std::endl

#define _OK(cond, has_diag, diag) \
    if (cond) \
        std::cout << "ok " << (++test_nr) << std::endl; \
    else { \
        failed_nr++; \
        std::cout << "not ok " << (++test_nr) << std::endl; \
        DIAG("  Failed test at " << __FILE__ << " line " << __LINE__ << "."); \
        if (has_diag) DIAG(diag); \
    }

#define OK(cond)        _OK((cond),     0, "")
#define NOK(cond)       _OK(!(cond),    0, "")
#define IS(a, b)        _OK((a) == (b), 1, (a))
#define ISNT(a, b)      _OK((a) != (b), 1, (a))
#define PASS()          _OK(true,       0, "")
#define FAIL()          _OK(false,      0, "")

#define DONE_TESTING() \
    std::cout << "1.." << test_nr << std::endl; \
    if (failed_nr == 0) { \
        return 0; \
    } else { \
        DIAG("Looks like you failed " << failed_nr << " test" << \
             (failed_nr > 1 ? "s" : "") << " of " << test_nr << "."); \
        return 1; \
    }

#endif // T_TAPTEST_H_
