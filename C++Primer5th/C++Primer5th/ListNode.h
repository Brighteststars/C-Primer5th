#ifndef LISTNODE_H
#define LISTNODE_H
#define  ListNodePosi(T) ListNode<T>*
typedef int Rank;

template< typename T>
struct ListNode{
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ;

	ListNode(){};
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s){}

	ListNodePosi(T) insertAsPred(T const& e);
	
	ListNodePosi(T) insertAsSucc(T const& e);
	
};


template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e){
	ListNodePosi(T) x = new ListNode(e, pred, this);
	pred->succ = x; pred = x;
	return x;
}


#endif 