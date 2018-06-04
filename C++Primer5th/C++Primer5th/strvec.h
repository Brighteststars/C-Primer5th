#ifndef STRVEC_H
#define STRVEC_H
#include <string>
#include <memory>
#include <utility>
#include <initializer_list>
#include <exception>
class StrVec
{
public:
	StrVec() :element(nullptr), first_free(nullptr), cap(nullptr){}
	StrVec(const StrVec &);
	StrVec(StrVec &&);
	StrVec &operator=(StrVec &&);
	StrVec(std::initializer_list<std::string> &);
	StrVec &operator=(const StrVec &);
	~StrVec();
	size_t size()const { return first_free - element; }
	size_t capacity()const { return cap - element; }
	void push_back(const std::string &);
	std::string *begin()const { return element; }
	std::string *end()const { return first_free; }
	bool operator==(const StrVec &);
	bool operator!=(const StrVec &);

	std::string &operator[](std::size_t n){ return element[n]; }
	const std::string &operator[](std::size_t n)const { return element[n]; }

	template<class... Args>
	inline void emplace_back(Args&... args)
	{
		check_n_alloc();
		alloc.construct(first_free++, std::forward<Args>(args)...);
	}
		
private:
	std::allocator<std::string> alloc;
	void chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}
	std::pair< std::string*,std::string*> alloc_n_copy(const std::string*, const std::string*);
	void free();
	void reallocate();
	std::string *element;
	std::string *first_free;
	std::string *cap;
};

std::pair< std::string*,  std::string*> StrVec::alloc_n_copy(const std::string *e, const std::string *f)
{
	auto data = alloc.allocate(f - e);
	return{ data, std::uninitialized_copy(e, f, data) };
}

void StrVec::free()
{
	if (element)
	{
		for (auto p = first_free; p != element;)
			alloc.destroy(--p);
		alloc.deallocate(element, cap - element);
	}
} 

void StrVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elem = element;
	for (size_t i = 0; i < size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();
	element = newdata;
	first_free = dest;
	cap = element + newcapacity;
}

void StrVec::push_back(const std::string &s)
{
	chk_n_alloc();
	alloc.construct(first_free++, s);
}

StrVec::StrVec(const StrVec &s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	element = newdata.first;
	first_free = cap = newdata.second;
}

StrVec &StrVec::operator=(const StrVec &s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	free();
	element = newdata.first;
	first_free = cap = newdata.second;
	return *this;
}

StrVec::StrVec(StrVec &&s) :element(s.element), first_free(s.first_free), cap(s.cap){}
{
	s.element = s.first_free = s.cap = nullptr;
}

StrVec &StrVec::operator=(StrVec &&s)
{
	if (this == &s)
		return *this;
	free();			//释放this对象内存
	element = s.element;
	first_free = s.first_free;
	cap = s.cap;
	s.element = s.first_free = s.cap = nullptr;
	return *this;
}

StrVec::~StrVec()
{
	free();
}

StrVec::StrVec(std::initializer_list<std::string> &il)
{
	auto newdata = alloc_n_copy(il.begin(), il.end());
	element = newdata.first;
	first_free = cap = newdata.second;
}

bool StrVec::operator==(const StrVec &s)
{
	if (size() != s.size())
		return false;
	auto it = element, it2 = s.element;
	while (it != first_free)
	{
		if (*it++ != *it2++)
			return false;
	}
	return true;
}

bool StrVec::operator!=(const StrVec &s)
{
	return !(*this == s);
}

#endif