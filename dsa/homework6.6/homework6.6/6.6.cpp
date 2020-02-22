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

void InsertList(List* l, int e)//空表，在尾节点前插入
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

int how_many_c(int c, List* tnodes)
{
	List now = tnodes[c];
	if (now._size == 0)
		return 1;
	else
	{
		ListNode* curr = now.header->Next;
		int childnum = 0;
		while (curr != now.trailer)
		{
			childnum += how_many_c(curr->data, tnodes);
			curr = curr->Next;
		}
		childnum++;
		return childnum;
	}
}

int main()
{
	int tnode_num;
	scanf_s("%d", &tnode_num);
	List* treenodes = (List*)calloc(tnode_num + 1, sizeof(List));
	for (int i = 1; i <= tnode_num; i++)
		CreateList(&(treenodes[i]));
	for (int i = 1; i <= tnode_num-1; i++)
	{
		int tmp;
		scanf_s("%d", &tmp);
		if (tmp)
			InsertList(&(treenodes[tmp]), i);
		scanf_s(" ");
	}
	int fin;
	scanf_s("%d", &fin);
	if (fin)
		InsertList(&(treenodes[fin]), tnode_num);
	for (int i = 1; i <= tnode_num; i++)
	{
		printf_s("%d", how_many_c(i, treenodes));
		printf_s(" ");
	}
}