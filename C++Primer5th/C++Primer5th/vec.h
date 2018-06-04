#ifndef VEC_H
#define VEC_H
#include <iostream>
#include <utility>	//std::move()
#include <initializer_list>
#include <memory>	//allocator


template<class T>
class Vec
{
	
public:
	Vec() :elements(nullptr), first_free(nullptr), cap(nullptr){}
	Vec(std::initializer_list<T> );
	size_t size()const { return first_free - elements; }
	size_t capacity()const { return cap - elements; }
	void push_back(const T &);
	T *begin()const { return elements; }
	T *end()const { return cap; }
	~Vec(){ free(); }

	Vec(const Vec &v);
	Vec &operator=(const Vec &v);
	Vec(Vec &&v);
	Vec &operator=(Vec &&v);

	T &operator[](size_t i) {return elements[i]; }
	const T &operator[](size_t i)const { return elements[i]; }
	
	bool operator==(const Vec &v);
	bool operator!=(const Vec &v);

	bool operator<(const Vec &v);
	bool operator>(const Vec &v);
	bool operator<=(const Vec &v);
	bool operator>=(const Vec &v);
private:
	std::allocator<T> alloc;		//static错误, alloc分配内存
	void chk_n_alloc() { if (size() == capacity()) reallocate(); }
	std::pair<T*, T*> alloc_n_copy(const T*, const T*);
	void free();
	void reallocate();

	T *elements;
	T *first_free;
	T *cap;
};

template<class T>
std::pair<T*, T*> Vec<T>::alloc_n_copy(const T*b, const T*e)
{
	auto data = alloc.allocate(e - b);
	return { data, std::uninitialized_copy(b, e, data) };
}

template<class T>
void Vec<T>::free()
{
	if (elements)
	{
		for (auto p = first_free; p != elements;)
			alloc.destroy(--p);
		alloc.deallocate(elements, cap - elements);
	}
}

template<class T>
void Vec<T>::reallocate()
{
	auto newcapacity = size() ? size() * 2 : 1;
	auto newdata = alloc.allocate(newcapacity);

	auto dest = newdata;
	auto ele = elements;
	for (int i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*ele++));
	free();

	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
}

template<class T>
Vec<T>::Vec(std::initializer_list<T> il)
{
	auto data = alloc_n_copy(il.begin(), il.end());
	elements = data.first;
	first_free = cap = data.second;
}

template<class T>
Vec<T>::Vec(const Vec &v)
{
	auto data = alloc_n_copy(v.begin(), v.end());
	elements = data.first;
	first_free = cap = data.second;
}

template<class T>
Vec<T> &Vec<T>::operator=(const Vec &v)
{
	if (this == &v)
		return *this;
	free();
	auto newdata = alloc_n_copy(v.begin(), v.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
	return *this;
}

template<class T>
Vec<T>::Vec(Vec &&t) : elements(t.elements), first_free(t.first_free), cap(t.cap)
{
	t.elements = t.first_free = t.cap = nullptr;
}

template<class T>
Vec<T> &Vec<T>::operator=(Vec &&t)
{
	if (this == &t)
		return *this;
	free();
	elements = t.elements;
	first_free = t.first_free;
	cap = t.cap;

	t.elements = t.first_free = t.cap = nullptr;
	return *this;
}

template<class T>
bool Vec<T>::operator==(const Vec &v)
{
	if (size() != v.size())
		return false;
	auto it = elements, its = v.elements;
	while (it != first_free)
	{
		if (*it++ != *its++)
			return false;
	}
	return true;
}

template<class T>
bool Vec<T>::operator!=(const Vec &v)
{
	return !(this == &v);
}

template<class T>
bool Vec<T>::operator < (const Vec &v)
{
	if (size() > v.size())
		return false;
	else if (size() < v.size())
		return true;
	for (auto it = elements, its = v.elements; it != first_free; ++it, ++its)
	{
		if (*it == *its)
			continue;
		else if (*it > *its)
			return true;
		else
			return false;
	}
	return false;		//两个相等
}

template<class T>
bool Vec<T>::operator>(const Vec &v)
{
	return !(this < &v) && *this != v;
}

template<class T>
bool Vec<T>::operator<=(const Vec &v)
{
	return !(this > &v);
}

template<class T>
bool Vec<T>::operator >= (const Vec &v)
{
	return !(this < &v);
}


#endif