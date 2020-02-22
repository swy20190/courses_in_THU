#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>


struct ListNode {
	int data1;
	int data2;
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

void InsertList(List* l, int e1,int e2)//空表，在尾节点前插入
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data1 = e1;
	np->data2 = e2;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

int main()
{
	int num;
	scanf_s("%d",&num);
	int e1, e2;
	List l;
	CreateList(&l);
	for (int i = 0; i < num; i++)
	{
		scanf_s("%d", &e1);
		scanf_s("%d", &e2);
		InsertList(&l, e1, e2);
	}
	int x;
	scanf_s("%d", &x);
	int sum = 0;
	ListNode* now = l.header;
	for (int i = 0; i < num; i++)
	{
		now = now->Next;
		sum = sum + now->data1*pow(x, now->data2);
	}
	printf_s("%d", sum);
}