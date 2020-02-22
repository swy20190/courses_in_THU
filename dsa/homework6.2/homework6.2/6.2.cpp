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
	char old = now->data;
	now->Next->Last = now->Last;
	l->header->Next = now->Next;
	free(now);
	l->_size--;
	return old;
}


List onion(char* inarr, int rank, int length)
{
	List relist;
	CreateList(&relist);
	if (rank * 2 > length)//左孩子溢出，说明是叶子
	{
		InsertList(&relist, inarr[rank]);
	}
	else if (rank * 2 + 1 > length)//右孩子溢出，单子节点
	{
		List L = onion(inarr, rank * 2, length);
		InsertList(&relist, inarr[rank]);
		InsertList(&relist, '(');
		while (L._size)
			InsertList(&relist, PopList(&L));
		InsertList(&relist, ')');
	}
	else//双子节点
	{
		List L = onion(inarr, rank * 2, length);
		List R = onion(inarr, rank * 2 + 1, length);
		InsertList(&relist, inarr[rank]);
		InsertList(&relist, '(');
		while (L._size)
			InsertList(&relist, PopList(&L));
		InsertList(&relist, ',');
		while (R._size)
			InsertList(&relist, PopList(&R));
		InsertList(&relist, ')');
	}
	return relist;
}

int main()
{
	List inlist;
	CreateList(&inlist);
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp == '$')
			break;
		else
			InsertList(&inlist, tmp);
	}
	int length = inlist._size / 2;
	char* pureinput = (char*)calloc(length + 1, sizeof(char));
	pureinput[0] = 0;
	for (int i = 1; i <= length; i++)
	{
		pureinput[i] = PopList(&inlist);
		PopList(&inlist);
	}
	List ans;
	CreateList(&ans);
	ans = onion(pureinput, 1, length);
	while (ans._size)
		printf_s("%c", PopList(&ans));
	return 0;
}