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

void PushList(List* l, char e)//入栈
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Last = l->header;
	np->Next = l->header->Next;
	np->Next->Last = np;
	l->header->Next = np;
	l->_size++;
}

char PopList(List* l)//出栈
{
	ListNode* now = l->header->Next;
	char old = now->data;
	now->Last->Next = now->Next;
	now->Next->Last = now->Last;
	free(now);
	l->_size--;
	return old;
}


int main()
{
	int N;//车厢总数
	scanf_s("%d",&N);
	int ready = 0;//已经处理完的车厢数
	List notready;
	CreateList(&notready);
	for (int i = 0; i < N+1; i++)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp != '\n')
			InsertList(&notready, tmp);
	}
	List train;//总栈
	CreateList(&train);
	char* out;
	out = (char*)calloc(2 * N, sizeof(char));
	int outhead = 0;
	while (1)
	{
		if ( ready == N)
			break;
		else
		{
			if ((train._size == 0||train.header->Next->data=='Y')&&notready._size!=0)
			{
				PushList(&train, PopList(&notready));
				out[outhead] = 'I';
			}
			else 
			{
				PopList(&train);
				ready++;
				out[outhead] = 'O';
			}
		}
		outhead++;
	}
	for (int i = 0; i < 2 * N; i++)
		printf_s("%c", out[i]);
}