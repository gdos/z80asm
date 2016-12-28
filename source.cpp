//-----------------------------------------------------------------------------
// z80asm source file
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "source.h"
#include "util.h"

//-----------------------------------------------------------------------------
// Source
//-----------------------------------------------------------------------------
Source::Source(bool keep_lines)
	: keep_lines_(keep_lines) {}

Source::~Source() {
	for (std::vector<SrcFile*>::iterator it = files_.begin(); it != files_.end(); ++it)
		delete *it;
	files_.clear();

	for (std::vector<SrcLine*>::iterator it = lines_.begin(); it != lines_.end(); ++it)
		delete *it;
	lines_.clear();
	stack_.clear();
}

bool Source::has_file(const std::string& filename) {
	for (std::vector<SrcFile*>::iterator it = stack_.begin(); it != stack_.end(); ++it)
		if ((*it)->filename() == filename)
			return true;
	return false;
}

bool Source::open(const std::string& filename) {
	SrcFile* file = new SrcFile(filename);
	if (file->good()) {
		files_.push_back(file);
		stack_.push_back(file);
		return true;
	}
	else {
		delete file;
		return false;
	}
}

SrcLine* Source::getline() {
	while (!stack_.empty()) {
		// delete previous line, unless keep_lines_
		if (!lines_.empty() && !keep_lines_)
			lines_.back()->clear_text();

		SrcLine* line = stack_.back()->getline();
		if (line != NULL) {
			// store new line
			lines_.push_back(line);
			return line;
		}
		else {							// end of this source
			stack_.pop_back();
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// SrcFile
//-----------------------------------------------------------------------------
SrcFile::SrcFile(const std::string& filename)
	: filename_(filename), ifs_(filename_.c_str(), std::ios::in | std::ios::binary), line_nr_(0) {
	if (!ifs_.good())
		std::perror(filename_.c_str());
}

SrcFile::~SrcFile() {
	ifs_.close();
}

SrcLine* SrcFile::getline() {
	std::string text;

	if (util::getline(ifs_, text).good()) {
		line_nr_++;
		return new SrcLine(this, line_nr_, text);
	}
	else {
		ifs_.close();
		return NULL;
	}
}

//-----------------------------------------------------------------------------
// SrcLine
//-----------------------------------------------------------------------------
SrcLine::SrcLine(SrcFile* src_file, int line_nr, std::string text)
	: src_file_(src_file), line_nr_(line_nr), text_(text) {}

SrcLine::~SrcLine() {
	// do not delete src_file_
}
