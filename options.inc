//-----------------------------------------------------------------------------
// z80asm global options
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

// #define BOOL_OPT(NAME, OPT, OPT_ARG, HELP)
// #define LIST_OPT(NAME, OPT, OPT_ARG, HELP)

BOOL_OPT(help,				"-h", "", "        Show help options")
BOOL_OPT(verbose,			"-v", "", "        Be verbose")

LIST_OPT(push_include_path,	"-I", "=PATH", "   Add directory to include search path")
LIST_OPT(push_library_path,	"-L", "=PATH", "   Add directory to library search path")

BOOL_OPT(do_list,			"-l", "", "        Create listing file.lis")
BOOL_OPT(swap_ix_iy,		"--IXIY", "", "    Swap IX and IY registers")
