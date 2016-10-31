//-----------------------------------------------------------------------------
// Error output
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "error.h"
#include "source.h"
#include "z80asm_config.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>

const char* kFatalMsg = "Fatal error: ";
const char* kErrorMsg = "Error: ";
const char* kWarnMsg = "Warning: ";

// formatting functions
static int location_col(SourceLine* line = NULL, const char* cursor = NULL) {
    if (line == NULL || cursor == NULL)
        return -1;

    const char* line_start = line->text.c_str();
    const char* line_end = line_start + line->text.size();
    if (cursor < line_start || cursor >= line_end)
        return -1;

    return cursor - line_start;
}

static void cerr_location(SourceLine* line = NULL, const char* cursor = NULL) {
	bool empty = true;

	if (line != NULL) {
        std::cerr << line->wptr_source->filename() << ':' << line->line_num << ':';
		empty = false;
	}

    int column = location_col(line, cursor);
	if (column >= 0) {
        std::cerr << column + 1 << ':';
		empty = false;
	}

    if (!empty)
        std::cerr << ' ';
}

static void cerr_token(SourceLine* line = NULL, const char* cursor = NULL) {
    const char* kEndOfLine = "end of line";

    int column = location_col(line, cursor);
    if (column < 0) {
        std::cerr << kEndOfLine;
    }
    else if (cursor == NULL) {
		std::cerr << kEndOfLine;
    }
    else if (*cursor == '\0' || *cursor == '\r' || *cursor == '\n') {
		std::cerr << kEndOfLine;
    }
    else if (*cursor < ' ' || *cursor >= '\x7e') {
        std::cerr << "'\\x" << std::hex << std::setw(2) << std::setfill('0') << (unsigned)*cursor << std::dec;
    }
    else {
        std::cerr << '\'' << *cursor << '\'';
    }
}

static void cerr_source_line(SourceLine* line = NULL, const char* cursor = NULL) {
    if (line == NULL)
        return;
    
    int column = location_col(line, cursor);
    std::cerr << "    " << line->text << std::endl;
    if (column >= 0) {
        std::cerr << "    ";
		for (std::string::iterator it = line->text.begin(); column >= 0 && it != line->text.end(); --column, ++it) {
			if (column == 0) {
				std::cerr << "^";
			}
			else if (*it == '\t') {
				std::cerr << *it;
			}
			else {
				std::cerr << ' ';
			}
		}
		std::cerr << std::endl;
    }
}

void cout_version() {
    std::cout << Z80ASM_NAME << " version " <<
        Z80ASM_VERSION_MAJOR << "." << Z80ASM_VERSION_MINOR << "." <<
        Z80ASM_VERSION_PATCH << "." << Z80ASM_VERSION_BUILD << " " <<
        Z80ASM_COPYRIGHT <<
        std::endl << std::endl;
}

void cout_usage() {
    std::cout << "Usage: " << Z80ASM_NAME << " [options] input_files" << std::endl;
}

void fatal_open_file(const std::string& filename) {
    std::cerr << kFatalMsg;
    std::perror(filename.c_str());
    exit(1);
}

void error_scan(SourceLine* line, const char* cursor) {
	cerr_location(line, cursor);
	std::cerr << "error: unexpected ";
	cerr_token(line, cursor);
	std::cerr << std::endl;
	cerr_source_line(line, cursor);
}
