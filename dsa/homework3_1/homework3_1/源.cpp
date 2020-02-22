#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

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


void PushList(List* l, int e)//入栈
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Last = l->header;
	np->Next = l->header->Next;
	np->Next->Last = np;
	l->header->Next = np;
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

struct cListNode {
	char data;
	cListNode* Next;
	cListNode* Last;
};

struct cList {
	cListNode* header;
	cListNode* trailer;
	int _size;
};

void cCreateList(cList* l)//build the list
{
	l->_size = 0;
	l->header = (cListNode*)malloc(sizeof(cListNode));
	l->trailer = (cListNode*)malloc(sizeof(cListNode));
	l->header->Next = l->trailer;
	l->header->Last = NULL;
	l->trailer->Last = l->header;
	l->trailer->Next = NULL;
}

void cInsertList(cList* l, char e)//空表，在尾节点前插入
{
	cListNode* np = (cListNode*)malloc(sizeof(cListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}


void cPushList(cList* l, char e)//入栈
{
	cListNode* np = (cListNode*)malloc(sizeof(cListNode));
	np->data = e;
	np->Last = l->header;
	np->Next = l->header->Next;
	np->Next->Last = np;
	l->header->Next = np;
	l->_size++;
}

char cPopList(cList* l)//出栈/出队
{
	cListNode* now = l->header->Next;
	char old = now->data;
	now->Next->Last = now->Last;
	l->header->Next = now->Next;
	free(now);
	l->_size--;
	return old;
}

char orderBetween(char top, char s)
{
	if (((top == '+' || top == '-') && (s == '*' || s == '/'))||((top=='&')&&(s!='&')))
		return '<';
	else if ((top == '&')&&(s == '&'))
		return '=';
	else 
		return '>';
}

int calcu(int op1, char op, int op2)
{
	if (op == '+')
		return op1 + op2;
	else if (op == '-')
		return op1 - op2;
	else if (op == '*')
		return op1*op2;
	else
		return op1 / op2;
}

int main()
{
	cList formula;
	cCreateList(&formula);
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp != '\n')
			cInsertList(&formula, tmp);
		else
			break;
	}
	cInsertList(&formula, '&');
	List opnd;//运算数栈
	cList optr;//运算符栈
	CreateList(&opnd);
	cCreateList(&optr);
	cPushList(&optr, '&');
	cListNode* fp = formula.header->Next;
	while (optr._size !=0 )
	{
		if ('0' <= fp->data&&fp->data <= '9')
		{
			PushList(&opnd, (int)fp->data - 48);
			fp = fp->Next;
		}
		else
			switch (orderBetween(optr.header->Next->data, fp->data))
			{
			case '<': {
				cPushList(&optr, fp->data);
				fp = fp->Next;
				break;
			}
			case '=': {
				cPopList(&optr);
				fp = fp->Next;
				break; 
			}
			case '>': {
				char op = cPopList(&optr);
				int popnd2 = PopList(&opnd);
				int popnd1 = PopList(&opnd);
				PushList(&opnd, calcu(popnd1, op, popnd2));
				break;
			}
			default:exit(-1);
			}
	}
	printf_s("%d", PopList(&opnd));
}
