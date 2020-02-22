#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>


struct ListNode {
	char data;
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

void InsertList(List* l, char e)//空表，在尾节点前插入
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

int main()
{
	List l1;
	List l2;
	CreateList(&l1);
	CreateList(&l2);
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp == '\n')
			break;
		else
			InsertList(&l1, tmp);
	}
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp == '\n')
			break;
		else
			InsertList(&l2, tmp);
	}
	printf_s("%d\n", l1._size);
	printf_s("%d", l2._size);
}