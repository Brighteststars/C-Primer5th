#ifndef BLOB_H
#define BLOB_H
#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
template<typename T> class Blob;
template<typename T> class BlobPtr;
template<typename T> bool operator==(const Blob<T> &, const Blob<T> &);

template<typename T>
class Blob
{
public:
	friend class BlobPtr<T>;
	friend bool operator==<T>(const Blob<T> &, const Blob<T> &);
	typedef typename std::vector<T>::size_type size_type;
	typedef T value_type;

	Blob();
	Blob(std::initializer_list<T> il);
	template<class U>
	Blob(U b, U e);
	size_type size()const { return data->size(); }
	bool empty()const { return data->empty(); }
	void push_back(const T &t)  { data->push_back(t); }
	void push_back(T &&t) { data->push_back(std::move(t)); }
	void pop_back();

	
	T &back();
	T &operator[](size_type i);
private:
	std::shared_ptr<std::vector<T>> data;
	void check(size_type i, const std::string &msg)const;
};

template<typename T>
Blob<T>::Blob() :data(std::make_shared<std::vector<T>>()){}

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il) : data(std::make_shared<std::vector<T>>(il)){}

template<typename T>
void Blob<T>::check(size_type i,const std::string &msg)const
{
	if (i >= data->size())
		throw std::out_of_range(msg);
}

template<typename T>
T &Blob<T>::back(){
	check(0, "back on empty Blob");
	return data->back();
}

template<typename T>
T &Blob<T>::operator[](size_type i)
{
	check(0, "subscript out of range");
	return (*data)[i];
}

template<typename T>
void Blob<T>::pop_back()
{
	check(0, "pop_back on empty Blob");
	data->pop_back();
}

template<typename T>
bool operator==(const Blob<T> &l, const Blob<T> &r)
{
	if (l.data != r.data)
		return false;
	return true;
}


template<typename T>
class BlobPtr
{
public:
	BlobPtr() :curr(0){}
	BlobPtr(Blob &b, size_t i = 0) :wptr(b.data), curr(i){}

	T &operator*()const {
	
		auto p = check(curr, "dereference past end");
		return (*p)[curr];
	}
	BlobPtr &operator++();
	BlobPtr &operator--();
private:
	std::shared_ptr<std::vector<T>> check(std::size_t i, const std::string &msg)const;
	std::weak_ptr<std::vector<T>> wptr;
	std::size_t curr;
};

template<typename T>
std::shared_ptr<std::vector<T>> BlobPtr<T>::check(std::size_t i, const std::string &msg)const
{
	auto ret = wptr.lock();				//确认未释放内存
	if (!ret)
		throw std::out_of_range("unbound BlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret;
}


template<typename T>
BlobPtr<T> &BlobPtr<T>::operator++()
{
	++curr;
	check(curr, "++obj error");
	return *this;
}

template<typename T>
BlobPtr<T> &BlobPtr<T>::operator--()
{
	--curr;
	if (curr < 0)
		throw std::out_of_range("--obj error");
	return *this;
}

template<typename T>
template<class U>
Blob<T>::Blob(U b, U e) : data(std::make_shared<std::vector<T>>(b, e)){}



#endif