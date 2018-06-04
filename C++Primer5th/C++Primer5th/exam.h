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
	auto temp = lb.bit ^ rb.bit;	//�ҳ���Ĵ�
	temp.flip();					//��ת����
	return temp.count();			//��������

}

#endif
