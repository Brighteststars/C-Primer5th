#ifndef EXAM_H
#define EXAM_H
#include <iostream>
#include <bitset>


template<unsigned N>
class exam
{

public:
	exam() :bit(){}
	size_t get_size(){ return N; }
	void updata(size_t n, bool b) { bit.set(n, b); }
	template<unsigned M>
	friend size_t score(const exam<M> &lb,const exam<M> &rb);
private:
	std::bitset<N> bit;
};

template<unsigned M>
size_t score(const exam<M> &lb,const exam<M> &rb)
{
	auto temp = lb.bit ^ rb.bit;	//找出错的答案
	temp.flip();					//翻转所以
	return temp.count();			//返回总数

}

#endif
