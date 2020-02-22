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

int main()
{
	List* hashes[26];
	for (int i = 0; i < 26; i++)
		hashes[i] = (List*)malloc(sizeof(List*));
	for (int i = 0; i < 26; i++)
		CreateList(hashes[i]);
	int n;
	scanf_s("%d", &n);
	char f;
	scanf_s("%c", &f);
	for (int i = 0; i < n; i++)
	{
		scanf_s("%c", &f,1);
		int ptr = int(f - 'A');
		InsertList(hashes[ptr], f);
		while (1)
		{
			char tmp;
			scanf_s("%c", &tmp);
			InsertList(hashes[ptr], tmp);
			if (tmp == '\n')
				break;
		}
	}
	for (int i = 0; i < 26; i++)
	{
		while (hashes[i]->_size)
		{
			printf_s("%c",PopList(hashes[i]));
		}
	}
}