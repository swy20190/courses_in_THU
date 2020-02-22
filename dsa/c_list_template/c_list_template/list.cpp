#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>


struct ListNode {
	int data;
	ListNode* Next;
	ListNode* Last;
};

struct List {
	ListNode* header;
	ListNode* trailer;
	int _size;
};

void CreateList(List* l)//build the list
{
	l->_size = 0;
	l->header = (ListNode*)malloc(sizeof(ListNode));
	l->trailer = (ListNode*)malloc(sizeof(ListNode));
	l->header->Next = l->trailer;
	l->header->Last = NULL;
	l->trailer->Last = l->header;
	l->trailer->Next = NULL;
}

void InsertList(List* l,int e)//空表，在尾节点前插入
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}
int GetElem(List* l, int i)//取第i位的元素
{
	ListNode* now = l->header->Next;
	for (int j = 0; j < i; j++)
		now = now->Next;
	return now->data;
}

void InsertBefore(int i, int e,List* l)//表不为空，在第i项前插入e
{
	ListNode* p = l->header->Next;
	for (int j = 0; j < i; j++)
		p = p->Next;
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = p;
	np->Last = p->Last;
	p->Last->Next = np;
	p->Last = np;
	l->_size++;
}

int DeleteElem(int r, List* l)//删除第r项
{
	ListNode* now = l->header->Next;
	for (int i = 0; i < r; i++)
		now = now->Next;
	int old = now->data;
	now->Last->Next = now->Next;
	now->Next->Last = now->Last;
	free(now);
	l->_size--;
	return old;
}

void PushList(List* l, int e)//入栈
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Last = l->header;
	np->Next = l->header->Next;
	np->Next->Last = np;
	l->header->Next = np;
	l->_size++;
}

int PopList(List* l)//出栈/出队
{
	ListNode* now = l->header->Next;
	int old = now->data;
	now->Next->Last = now->Last;
	l->header->Next = now->Next;
	free(now);
	l->_size--;
	return old;
}

void enQ(List* l, int e)//入队
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	np->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

int main()
{
	List l1;
	CreateList(&l1);
	for (int i = 0; i < 514; i++)
		InsertList(&l1, i + 1);
	DeleteElem(114, &l1);
	printf_s("%d\n", DeleteElem(114,&l1));
	printf_s("%d\n", l1._size);
	return 0;
}