#pragma once
#include <stdio.h>
//listnode
//只保留插入排序,默认构造函数
typedef int Rank;
#define ListNodePosi(T) ListNode<T>*

template <typename T> struct ListNode {
	T data;
	ListNodePosi(T) pred;
	ListNodePosi(T) succ;
	ListNode() {}
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		:data(e), pred(p), succ(s) {}
	ListNodePosi(T) insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);
};

template <typename T> class List {
public:
	int _size;
	ListNodePosi(T) header;
	ListNodePosi(T) trailer;

	void init();
	int clear();
	void insertionSort(ListNodePosi(T) p, int n);
	//构造函数
	List() { init(); }
	
	//析构函数
	~List();
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[](Rank r)const;
	ListNodePosi(T) first() const { return header->succ; }
	ListNodePosi(T) last() const { return trailer->pred; }
	//判断节点对外合法
	bool valid(ListNodePosi(T) p)
	{
		return p && (trailer != p) && (header != p);
	}
	int disordered() const;
	ListNodePosi(T) find(T const& e)const
	{
		return find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;
	ListNodePosi(T) search(T const& e) const
	{
		return search(e, _size, trailer);
	}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;

	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);
	void sort(ListNodePosi(T) p, int n);
	void sort() { sort(first(), _size); }
};

template <typename T> void List<T>::init()
{
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer;
	header->pred = NULL;
	trailer->pred = header;
	trailer->succ = NULL;
	_size = 0;
}

template <typename T>
T& List<T>::operator[](Rank r) const
{
	ListNodePosi(T) p = first();
	while (0 < r--)
		p = p->succ;
	return p->data;
}

template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p)const
{
	while (0 < n--)
		if (e == (p == p->pred)->data)
			return p;
	return NULL;
}

template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
	_size++;
	return header->insertAsSucc(e);
}
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
	_size++;
	return trailer->insertAsPred(e);
}
template <typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const&e)
{
	_size++;
	return p->insertAsSucc(e);
}
template <typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const&e)
{
	_size++;
	return p->insertAsPred(e);
}

template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, pred, this);
	pred->succ = x;
	pred = x;
	return x;
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, this, succ);
	succ->pred = x;
	succ = x;
	return x;
}
template <typename T>
T List<T>::remove(ListNodePosi(T) p)
{
	T e = p->data;
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p;
	_size--;
	return e;
}

template <typename T>
List<T>::~List()
{
	clear();
	delete header;
	delete trailer;
}

template <typename T>
int List<T>::clear()
{
	int oldSize = _size;
	while (0 < _size)
		remove(header->succ);
	return oldSize;
}



template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n)
{
	insertionSort(p, n);
}

template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
	for (int r = 0; r < n; r++)
	{
		insertA(search(p->data, r, p), p->data);
		p = p->succ;
		remove(p->pred);
	}
}

template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const
{
	while (0 <= n--)
		if (((p = p->pred)->data) <= e)
			break;
	return p;
}