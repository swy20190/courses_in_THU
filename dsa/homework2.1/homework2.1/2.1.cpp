#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>


struct cListNode {
	char data;
	struct cListNode* Next;
	struct cListNode* Last;
};

struct cList {
	struct cListNode* header;
	struct cListNode* trailer;
	int _size;
};

void cCreateList(struct cList* l)//build the list
{
	l->_size = 0;
	l->header = (struct cListNode*)malloc(sizeof(struct cListNode));
	l->trailer = (struct cListNode*)malloc(sizeof(struct cListNode));
	l->header->Next = l->trailer;
	l->header->Last = NULL;
	l->trailer->Last = l->header;
	l->trailer->Next = NULL;
}

void cInsertList(struct cList* l, char e)//空表，在尾节点前插入
{
	struct cListNode* np = (struct cListNode*)malloc(sizeof(struct cListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

int main()
{
	cList numlist;
	cList callist;
	cList allist;
	cList mainlist;
	cCreateList(&numlist);
	cCreateList(&callist);
	cCreateList(&allist);
	cCreateList(&mainlist);
	int len;
	scanf_s("%d", &len);
	int count = 0;
	while (1)
	{
		if (count > len)
			break;
		else
		{
			char tmp;
			scanf_s("%c", &tmp);
			if (tmp == '\n' || tmp == ' ')
				count++;
			cInsertList(&mainlist, tmp);
		}
	}
	mainlist.header->Next->data = ' ';
	mainlist.trailer->Last->data = ' ';
	cListNode* now;
	now = mainlist.header;
	int direct = 0;
	while (1)
	{
		now = now->Next;
		if (now->Next==mainlist.trailer)
			break;
		else
		{
			if (now->data ==' ')
			{
				if ((now->Next->data == '+' || now->Next->data == '-' || now->Next->data == '*' || now->Next->data == '/') && now->Next->Next->data==' ')
					direct = 1;
				else if (now->Next->data >= 'A'&&now->Next->data <= 'Z')
					direct = 2;
				else
					direct = 3;
			}
			if (direct == 1)
				cInsertList(&callist, now->data);
			else if (direct == 2)
				cInsertList(&allist, now->data);
			else
				cInsertList(&numlist, now->data);
		}
	}
	now = allist.header->Next->Next;
	while (now != allist.trailer)
	{
		printf_s("%c", now->data);
		now = now->Next;
	}
	printf_s("\n");
	now = numlist.header->Next->Next;
	while (now != numlist.trailer)
	{
		printf_s("%c", now->data);
		now = now->Next;
	}
	printf_s("\n");
	now = callist.header->Next->Next;
	while (now != callist.trailer)
	{
		printf_s("%c", now->data);
		now = now->Next;
	}
}