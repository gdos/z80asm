//-----------------------------------------------------------------------------
// z80asm source file
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "source.h"
#include "util.h"

SourceFile::SourceFile(const std::string& filename, bool keep_lines)
	: filename_(filename)
	, ifs_(filename_.c_str(), std::ios::in | std::ios::binary)
	, keep_lines_(keep_lines)
{
	if (!ifs_.good())
		std::perror(filename_.c_str());
}

SourceFile::~SourceFile()
{
	ifs_.close();
	for (std::vector<SourceLine*>::iterator it = lines_.begin(); it != lines_.end(); ++it)
		delete *it;
	lines_.clear();
}

SourceLine* SourceFile::getline()
{
	std::string text;

	if (safe_getline(ifs_, text).good()) {
		// delete previous lines, unless keep_lines_
		if (!lines_.empty() && !keep_lines_)
			lines_.back()->text = std::string();

		// allocate a new SourceLine and append to vector
		int line_num = lines_.size() + 1;
		lines_.push_back(new SourceLine(this, line_num, text));

		return lines_.back();
	}
	else {
		ifs_.close();
		return NULL;
	}
}

SourceStack::SourceStack(bool keep_lines)
	: keep_lines_(keep_lines)
{
}

SourceStack::~SourceStack()
{
	weak_ptr_lines.clear();
	weak_ptr_stack.clear();

	for (std::vector<SourceFile*>::iterator it = files.begin(); it != files.end(); ++it)
		delete *it;
	files.clear();
}

bool SourceStack::has_file(const std::string& filename)
{
	for (std::vector<SourceFile*>::iterator it = weak_ptr_stack.begin(); it != weak_ptr_stack.end(); ++it)
		if ((*it)->filename() == filename)
			return true;
	return false;
}

bool SourceStack::open(const std::string& filename)
{
	SourceFile* file = new SourceFile(filename, keep_lines_);
	if (file->good()) {
		files.push_back(file);
		weak_ptr_stack.push_back(file);
		return true;
	}
	else {
		delete file;
		return false;
	}
}

SourceLine* SourceStack::getline()
{
	while (!weak_ptr_stack.empty()) {
		SourceLine* pline = weak_ptr_stack.back()->getline();
		if (pline != NULL) {
			weak_ptr_lines.push_back(pline);
			return pline;
		}
		else {							// end of this source
			weak_ptr_stack.pop_back();
		}
	}
	return NULL;
}