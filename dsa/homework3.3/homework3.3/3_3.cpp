#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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
	List l;
	CreateList(&l);
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp != '@')
			InsertList(&l, tmp);
		else
			break;
	}
	int flag = 1;
	ListNode* front = l.header;
	ListNode* back = l.trailer;
	for (int i = 0; i < l._size / 2; i++)
	{
		front = front->Next;
		back = back->Last;
		if (front->data != back->data)
		{
			flag = 0;
			break;
		}
	}
	if (flag == 1)
		printf_s("true");
	else
		printf_s("false");
	return 0;
}