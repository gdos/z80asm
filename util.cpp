//-----------------------------------------------------------------------------
// z80asm - utilities
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "memcheck.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <cstdio>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>


namespace util {

static bool check_result_perror(int result, const std::string& file) {
	if (result != 0) {
		std::perror(file.c_str());
		return false;
	}
	else {
		return true;
	}
}

// getline function that handles all three line endings ("\r", "\n" and "\r\n"):
// http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream& getline(std::istream& is, std::string& t) {
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);

			return is;
		default:
			t += (char)c;
		}
	}
}

void spew(const std::string& file, const std::string& text) {
	std::ofstream ofs(file.c_str(), std::ios::out | std::ios::binary);
	if (!ofs.good()) {
		std::perror(file.c_str());
	}
	else {
		ofs << text;
		ofs.close();
	}
}

std::string slurp(const std::string& file) {
	std::string ret, line;

	std::ifstream ifs(file.c_str(), std::ios::binary);
	if (!ifs.good()) {
		std::perror(file.c_str());
		return ret;
	}
	else {
		while (!getline(ifs, line).eof()) {
			ret.append(line);
			ret.append("\n");
		}
		return ret;
	}
}

bool mkdir(const std::string& directory) {
	if (is_dir(directory))
		return true;

#ifdef _WIN32
	return check_result_perror(_mkdir(directory.c_str()), directory);
#else
	return check_result_perror(::mkdir(directory.c_str(), 0733), directory);
#endif
}

bool rmdir(const std::string& directory) {
	if (!file_exists(directory))
		return true;

#ifdef _WIN32
	return check_result_perror(_rmdir(directory.c_str()), directory);
#else
	return check_result_perror(::rmdir(directory.c_str()), directory);
#endif
}

bool remove(const std::string& file) {
	if (!file_exists(file))
		return true;

#ifdef _WIN32
	return check_result_perror(_unlink(file.c_str()), file);
#else
	return check_result_perror(unlink(file.c_str()), file);
#endif
}

bool file_exists(const std::string& file) {
	struct stat sb;
	return stat(file.c_str(), &sb) == 0;
}

int file_size(const std::string& file) {
	if (!is_file(file))
		return -1;

	struct stat sb;
	if (stat(file.c_str(), &sb) != 0) {
		std::perror(file.c_str());
		return -1;
	}
	else {
		return static_cast<int>(sb.st_size);
	}
}

bool is_file(const std::string& file) {
#ifdef _WIN32
	DWORD attr = GetFileAttributesA(file.c_str());
	return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
#else
	struct stat sb;
	if (stat(file.c_str(), &sb) != 0)
		return false;
	else
		return S_ISREG(sb.st_mode);
#endif
}

bool is_dir(const std::string& file) {
#ifdef _WIN32
	DWORD result = GetFileAttributesA(file.c_str());
	if (result == INVALID_FILE_ATTRIBUTES)
		return false;
	else
		return (result & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
	struct stat sb;
	if (stat(file.c_str(), &sb) != 0)
		return false;
	else
		return S_ISDIR(sb.st_mode);
#endif
}

std::string file_search(const std::string& file, const std::vector<std::string>& dirs) {

	if (is_file(file))
		return file;

	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it) {
		std::string dir = *it;
		if (is_dir(dir)) {
			std::string path = dir + "/" + file;
			if (is_file(path))
				return path;
		}
	}

	// not found, return original
	return file;
}

}; // namespace

