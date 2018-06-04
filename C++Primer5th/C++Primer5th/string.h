#ifndef STRING_H
#define STRING_H
#include <memory>
#include <utility>
#include <iostream>

class String
{
	friend std::ostream &operator<<(std::ostream &, const String &);
public:
	String() :elements(nullptr), first_free(nullptr), cap(nullptr){}
	String(const String &);
	String(const char*);
	String &operator=(const String &);
	String &operator=(const char*);
	String(String &&);
	String &operator=(String &&);
	~String(){ free(); }

	size_t capacity()const{ return cap - elements; }
	size_t size()const { return first_free - elements; }
	char* begin()const { return elements; }
	char* end()const { return first_free; }
	bool operator==(const String &);
	bool operator!=(const String &);

	char &operator[](std::size_t n){ return elements[n]; }
	const char &operator[](std::size_t n)const { return elements[n]; }
private:
	std::allocator<char> alloc;
	void chk_n_alloc();
	void reallocate();
	void free();
	std::pair<char*, char*> alloc_n_copy(const char*, const char*);
	char *elements;
	char *first_free;
	char *cap;
};

void String::chk_n_alloc()
{
	if (first_free == cap)
		reallocate();
	
}

void String::free()
{
	if (elements)
	{
		for (auto p = first_free; p != elements;)
			alloc.destroy(--p);
		alloc.deallocate(elements, cap - elements);
	}
}

std::pair<char*, char*> String::alloc_n_copy(const char *b, const char *e)
{
	auto newdata = alloc.allocate(e - b);
	return{ newdata, std::uninitialized_copy(b, e, newdata) };
}

void String::reallocate()
{
	auto newcapacity = size() ? size() * 2 : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();
	elements = newdata;
	first_free = elem;
	cap = elements + newcapacity;
}

String::String(const String &s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

String &String::operator=(const String &s)
{
	if (this == &s)
		return *this;
	auto p = alloc_n_copy(s.begin(), s.end());
	free();
	elements = p.first;
	first_free = cap = p.second;
	return *this;
}

String::String(const char* str)
{
	auto p = alloc_n_copy(str, str + strlen(str) + 1);
	elements = p.first;
	first_free = cap = p.second;
}

String &String::operator=(const char* str)
{
	auto p = alloc_n_copy(str, str + strlen(str) + 1);
	free();
	elements = p.first;
	first_free = cap = p.second;
	return *this;
}

String::String(String &&s) :elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;
}

String &String::operator=(String &&s)
{
	if (this == &s)
		return *this;
	free();
	elements = s.elements;
	first_free = s.first_free;
	cap = s.cap;
	s.elements = s.first_free = s.cap = nullptr;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const String &s)
{
	os << s.begin();
	return os;
}

bool String::operator==(const String &s)
{
	if (size() != s.size())
		return false;
	auto it = elements, it2 = s.elements;
	while (it != first_free)
	{
		if (*it++ != *it2++)
			return false;
	}
	return true;
}

bool String::operator!=(const String &s)
{
	return !(*this == s);
}

#endif