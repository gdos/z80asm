//-----------------------------------------------------------------------------
// z80asm source file
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "source.h"
#include "util.h"

SourceFile::SourceFile(const std::string& filename_, bool keep_lines_)
	: filename(filename_)
	, ifs(filename.c_str(), std::ios::in | std::ios::binary)
	, keep_lines(keep_lines_)
{
	if (!ifs.good())
		std::perror(filename.c_str());
}

SourceFile::~SourceFile()
{
	ifs.close();
	for (std::vector<SourceLine *>::iterator it = lines.begin(); it != lines.end(); ++it)
		delete *it;
	lines.clear();
}

SourceLine *SourceFile::getline()
{
	std::string text;

	if (safe_getline(ifs, text).good()) {
		// delete previous lines, unless m_keep_lines
		if (!lines.empty() && !keep_lines)
			lines.back()->text = std::string();

		// allocate a new SourceLine and append to vector
		unsigned line_nr = lines.size() + 1;
		lines.push_back(new SourceLine(this, line_nr, text));

		return lines.back();
	}
	else {
		ifs.close();
		return NULL;
	}
}

SourceStack::SourceStack(bool keep_lines_)
	: keep_lines(keep_lines_)
{
}

SourceStack::~SourceStack()
{
	plines.clear();		// weak pointers
	pstack.clear();		// weak pointers

	for (std::vector<SourceFile *>::iterator it = files.begin(); it != files.end(); ++it)
		delete *it;
	files.clear();
}

bool SourceStack::has_file(const std::string& filename)
{
	for (std::vector<SourceFile *>::iterator it = pstack.begin(); it != pstack.end(); ++it)
		if ((*it)->get_filename() == filename)
			return true;
	return false;
}

bool SourceStack::open(const std::string& filename)
{
	SourceFile *file = new SourceFile(filename, keep_lines);
	if (file->good()) {
		files.push_back(file);
		pstack.push_back(file);
		return true;
	}
	else {
		delete file;
		return false;
	}
}

SourceLine *SourceStack::getline()
{
	while (!pstack.empty()) {
		SourceLine *pline = pstack.back()->getline();
		if (pline != NULL) {
			plines.push_back(pline);
			return pline;
		}
		else {							// end of this source
			pstack.pop_back();			// weak pointer, no delete
		}
	}
	return NULL;
}
