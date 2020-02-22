#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

struct Queen {
	int x;
	int y;
};

struct ListNode {
	Queen q;
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

void InsertList(List* l, Queen e)//空表，在尾节点前插入
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->q.x = e.x;
	np->q.y = e.y;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

void InsertBefore(int i, Queen e, List* l)//表不为空，在第i项前插入e
{
	ListNode* p = l->header->Next;
	for (int j = 0; j < i; j++)
		p = p->Next;
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->q.x = e.x;
	np->q.y = e.y;
	np->Next = p;
	np->Last = p->Last;
	p->Last->Next = np;
	p->Last = np;
	l->_size++;
}


void PushList(List* l, Queen e)//入栈
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->q.x = e.x;
	np->q.y = e.y;
	np->Last = l->header;
	np->Next = l->header->Next;
	np->Next->Last = np;
	l->header->Next = np;
	l->_size++;
}

Queen PopList(List* l)//出栈/出队
{
	ListNode* now = l->header->Next;
	Queen old = now->q;
	now->Next->Last = now->Last;
	l->header->Next = now->Next;
	free(now);
	l->_size--;
	return old;
}


int placeQ(int N)
{
	int nsolution = 0;
	List solution;
	CreateList(&solution);
	Queen q;
	q.x = 0;
	q.y = 0;
	int* xarray = (int*)calloc(N, sizeof(int));
	int* yarray = (int*)calloc(N, sizeof(int));
	int* sumarray = (int*)calloc(2 * N, sizeof(int));
	int* diffarray = (int*)calloc(2 * N, sizeof(int));
	for (int i = 0; i < N; i++)
		xarray[i] = yarray[i] = 0;
	for (int i = 0; i < 2 * N; i++)
		sumarray[i] = diffarray[i] = 0;
	do
	{
		if (N <= solution._size || N <= q.y)
		{
			Queen tmp = PopList(&solution);
			q.x = tmp.x;
			q.y = tmp.y;
			xarray[q.x] = 0;
			yarray[q.y] = 0;
			sumarray[q.x + q.y] = 0;
			diffarray[q.x - q.y+N] = 0;
			q.y++;
		 }
		else
		{
			while ((q.y < N) && ((xarray[q.x] == 1) || (yarray[q.y] == 1) || (sumarray[q.x + q.y] == 1) || (diffarray[q.x - q.y+N] == 1)))
			{
				q.y++;
			}
			if (N > q.y)
			{
				PushList(&solution, q);
				xarray[q.x] = 1;
				yarray[q.y] = 1;
				sumarray[q.x + q.y] = 1;
				diffarray[q.x - q.y+N] = 1;
				if (N <= solution._size)
					nsolution++;
				q.x++;
				q.y = 0;
			}
		}
	} while ((0 < q.x) || (q.y < N));
	return nsolution;
}


int main()
{
	int num;
	scanf_s("%d", &num);
	printf_s("%d", placeQ(num));
	return 0;
}