#ifndef MAKE_PLURAL
#define MAKE_PLURAL
#include <string>
std::string make_plural(size_t sz, const std::string word, const std::string end)
{
	return sz > 1 ? word + end : word;
}

#endif