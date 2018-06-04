#ifndef STRBLOB_H
#define STRBLOB_H
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <stdexcept>

class StrBlobPtr;
class StrBlob
{
	friend class StrBlobPtr;			//可以使用StrBlob私有成员
public:
	typedef std::vector<std::string>::size_type size_type;			//vector<string>的index
	StrBlob();
	StrBlob(std::initializer_list<std::string> il);
	StrBlob(std::vector<std::string> *p) :data(p){}
	StrBlob(const StrBlob &s) :data(std::make_shared < std::vector<std::string>>(*s.data)){}		//copy constructor
	StrBlob &operator=(const StrBlob &);

	size_type size() const{ return data->size(); }			//返回vector<string>对象的size
	bool empty() const { return data->empty(); }
	void push_back(const std::string &t){ data->push_back(t); }
	void pop_back();
	std::string &front();
	const std::string &front()const;
	std::string &back();
	const std::string &back()const;
	const std::shared_ptr<std::vector<std::string>> &shared_ptr()const { return data; }		//返回data的const引用

	StrBlobPtr begin();			//使用StrBlobPtr构造函数,使用了StrBlob私有成员,所以StrBlobPtr必须是友元函数
	StrBlobPtr end();
	std::string &operator[](std::size_t n){ return data->at(n); }
	const std::string &operator[](std::size_t n)const { return data->at(n); }

private:
	std::shared_ptr<std::vector<std::string>> data;						//指向vector<string> 对象的智能指针data
	void check(size_type i, const std::string &msg)const;				//检查shared_ptr指向vector<string>对象的size是否为0
};

class StrBlobPtr
{
	friend bool eq(const StrBlobPtr &, const StrBlobPtr &);
public:
	StrBlobPtr() : curr(0){}
	StrBlobPtr(StrBlob &a, std::size_t sz = 0) :wptr(a.data), curr(sz){}

	std::string &deref(int off)const;					//decrement间接引用
	std::string &deref()const;
	StrBlobPtr &incr();

	std::string &operator*()const;
	std::string* operator->()const;
	std::string  &operator[](std::size_t n){ return wptr.lock()->at(n); }
	const std::string &operator[](std::size_t n)const{ return wptr.lock()->at(n); }

	StrBlobPtr &operator++();		//前++
	StrBlobPtr &operator--();
	StrBlobPtr operator++(int);		//后++
	StrBlobPtr operator--(int);
	StrBlobPtr operator+(std::size_t);
	StrBlobPtr operator-(std::size_t);
private:
	std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string &)const;
	std::weak_ptr<std::vector<std::string>> wptr;
	std::size_t curr;
};

StrBlob::StrBlob() : data(std::make_shared<std::vector<std::string>>()){}
StrBlob::StrBlob(std::initializer_list<std::string> il) : data(std::make_shared<std::vector<std::string>>(il)){}

StrBlob &StrBlob::operator=(const StrBlob &s)						//copy assignment operator
{
	data = std::make_shared<std::vector<std::string>>(*s.data);
	return *this;
}

void StrBlob::check(size_type i, const std::string &msg)const
{
	if (i >= data->size())
		throw std::out_of_range(msg);
}

std::string &StrBlob::front()
{
	check(0, "front on empty StrBlob");
	return data->front();
}

const std::string &StrBlob::front()const
{
	check(0, "front on empty StrBlob");
	return data->front();
	
}

std::string &StrBlob::back()
{
	check(0, "back on empty StrBlob");
	return data->back();
}

const std::string &StrBlob::back()const
{
	check(0, "back on empty StrBlob");
	return data->back();
}

void StrBlob::pop_back()
{
	check(0, "pop_back on empty StrBlob");
	data->pop_back();
}

StrBlobPtr StrBlob::begin()
{
	return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
	return StrBlobPtr(*this, data->size());
}

std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(std::size_t i, const std::string &msg)const
{
	auto ret = wptr.lock();
	if (!ret)
		throw std::runtime_error("unbound StrBlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);

	return ret;
}

std::string &StrBlobPtr::deref()const
{
	auto p = check(curr, "dereference past end");

	return (*p)[curr];
}

std::string &StrBlobPtr::deref(int off)const
{
	auto p = check(curr + off, "dereference past end");

	return (*p)[curr + off];
}

StrBlobPtr &StrBlobPtr::incr()
{
	check(curr, "increment past end of StrBlobPtr");
	++curr;
	return *this;
}

bool eq(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
	auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
	if (l == r)
		return (!r || lhs.curr == rhs.curr);
	else
		return false;
}

bool neq(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
	return !eq(lhs, rhs);
}

StrBlobPtr &StrBlobPtr::operator++()
{
	check(curr, "increment past end of StrBlobPtr");
	++curr;
	return *this;
}

StrBlobPtr &StrBlobPtr::operator--()
{
	--curr;
	check(curr, "decrement past begin of StrBlobPtr");
	return *this;
}

StrBlobPtr StrBlobPtr::operator++(int)
{
	StrBlobPtr ret = *this;
	++*this;
	return ret;
}

StrBlobPtr StrBlobPtr::operator--(int)
{
	StrBlobPtr ret = *this;
	--*this;
	return ret;
}

StrBlobPtr StrBlobPtr::operator+(std::size_t n)
{
	StrBlobPtr ret = *this;
	curr += n;
	return ret;
}

StrBlobPtr StrBlobPtr::operator-(std::size_t n)
{
	StrBlobPtr ret = *this;
	curr -= n;
	return ret;
}

std::string &StrBlobPtr::operator*()const
{
	auto p = check(curr, "dereference past end");
	return (*p)[curr];
}

std::string *StrBlobPtr::operator->()const
{
	return & this->operator*();
}

#endif