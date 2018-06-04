#include <iostream>
typedef int Rank;
#define  DEFAULT_CAPACITY 3

template<typename T>
class vector{
protected:
	Rank _size; int _capacity; T* _elem;
	void mergeSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
};


template<typename T>
void vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2) return;
	Rank mi = (hi + lo) / 2;
	mergeSort(lo, mi); mergeSort(mi, hi);
	merge(lo, mi, hi);
}


template<typename T>
void vector<T>::merge(Rank lo,Rank mi,Rank hi)
{
	T* A = _elem + lo;
	int lb = mi - lo; T* B = new T[lb];
	for (int i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi; T* C = _elem + mi;
	for (int i = 0, j = 0, k = 0; j < lb;){
		if (k < lc  && B[j] > C[k]) A[i++] = C[k++];
		if (k >= lc || C[k] >= B[j]) A[i++] = B[j++];
	}
	delete[] B;
}