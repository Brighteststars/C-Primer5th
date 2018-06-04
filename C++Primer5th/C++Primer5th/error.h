#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>

class out_of_stock :public std::runtime_error
{
public:
	explicit out_of_stock(const std::string &s) :runtime_error(s){}
};

class isbn_mismatch :public std::logic_error
{
public:
	explicit isbn_mismatch(const std::string &s) :logic_error(s){}
	isbn_mismatch(const std::string &s, const std::string &lhs, const std::string &rhs)
		:logic_error(s), left(lhs), right(rhs){}
	std::string left, right;
};

#endif