#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

int hms[1000005] = { 0 };

struct ListNode {
	int data;
	struct ListNode* Next;
	struct ListNode* Last;
};

struct List {
	struct ListNode* header;
	struct ListNode* trailer;
	int _size;
};

void CreateList(struct List* l)//build the list
{
	l->_size = 0;
	l->header = (struct ListNode*)malloc(sizeof(struct ListNode));
	l->trailer = (struct ListNode*)malloc(sizeof(struct ListNode));
	l->header->Next = l->trailer;
	l->header->Last = NULL;
	l->trailer->Last = l->header;
	l->trailer->Next = NULL;
}

void InsertList(struct List* l, int e)//空表，在尾节点前插入
{
	struct ListNode* np = (struct ListNode*)malloc(sizeof(struct ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

int how_many_c(int c, struct List* tnodes)
{
	if (hms[c])
		return hms[c];
	else {
		struct List now = tnodes[c];
		if (now._size == 0)
		{
			hms[c] = 1;
			return 1;
		}
		else
		{
			struct ListNode* curr = now.header->Next;
			int childnum = 0;
			while (curr != now.trailer)
			{
				childnum += how_many_c(curr->data, tnodes);
				curr = curr->Next;
			}
			childnum++;
			hms[c] = childnum;
			return childnum;
		}
	}
}

int main()
{
	int tnode_num;
	scanf("%d", &tnode_num);
	struct List* treenodes = (struct List*)calloc(tnode_num + 1, sizeof(struct List));
	for (int i = 1; i <= tnode_num; i++)
		CreateList(&(treenodes[i]));
	for (int i = 1; i <= tnode_num - 1; i++)
	{
		int tmp;
		scanf("%d", &tmp);
		if (tmp)
			InsertList(&(treenodes[tmp]), i);
		scanf(" ");
	}
	int fin;
	scanf("%d", &fin);
	if (fin)
		InsertList(&(treenodes[fin]), tnode_num);
	for (int i = 1; i <= tnode_num; i++)
	{
		printf("%d", how_many_c(i, treenodes));
		printf(" ");
	}
}