#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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

struct treenode {
	int parent;
	List children;
};

int main()
{
	int tnode_num;
	scanf_s("%d", &tnode_num);
	treenode* treenodes = (treenode*)calloc(tnode_num + 1, sizeof(treenode));
	for (int i = 1; i <= tnode_num; i++)
		CreateList(&(treenodes[i].children));
	for (int i = 1; i <= tnode_num; i++)
	{
		int tmp;
		scanf_s("%d", &tmp);
		if (tmp)
		{
			treenodes[i].parent = tmp;
			InsertList(&(treenodes[tmp].children), i);
		}
	}
	for (int i = 1; i <= tnode_num; i++)
	{
		if (treenodes[i].children._size)
			printf_s("%d", treenodes[i].children.header->Next->data);
		else
			printf_s("0");
		printf_s(" ");
		int par = treenodes[i].parent;
		ListNode* now = treenodes[par].children.header;
		while ((now != treenodes[par].children.trailer) && (now->data != i))
			now = now->Next;
		if ((now == treenodes[par].children.trailer) || (now->Next == treenodes[par].children.trailer))
			printf_s("0");
		else
			printf_s("%d", now->Next->data);
		printf_s("\n");
	}
}