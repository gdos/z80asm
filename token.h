//-----------------------------------------------------------------------------
// z80asm scanner tokens
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#ifndef TOKEN_H_
#define TOKEN_H_

#include "memcheck.h"
#include "fwd.h"
#include <string>

class Token : noncopyable {
public:
	Token();
	Token(int num_value);
	Token(const std::string& str_value);
	virtual ~Token();

	int id() const { return id_; }
	int num_value() const { return num_value_; }
	const std::string& str_value() const { return str_value_; }

	static void free_token(Token* token);

private:
	int				id_;
	int				num_value_;
	std::string		str_value_;
};

#endif // TOKEN_H_
