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


struct treenode {
	treenode* parent=NULL;
	treenode* lchild = NULL;
	treenode* rchild = NULL;
	List data;
};

void out_tree(treenode* T)
{
	if (T)
	{
		out_tree(T->lchild);
		out_tree(T->rchild);
		int flag = T->data._size;
		while (T->data._size)
			printf_s("%c", PopList(&(T->data)));
		if (flag)
			printf_s(",");
	}
	return;
}

int main()
{
	treenode* root = (treenode*)malloc(sizeof(treenode));
	CreateList(&(root->data));
	treenode* curr = root;
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp!='\n')
		{
			if (tmp == '(')
			{
				curr->lchild = (treenode*)malloc(sizeof(treenode));
				curr->lchild->parent = curr;
				curr = curr->lchild;
				curr->lchild = NULL;
				curr->rchild = NULL;
				CreateList(&(curr->data));
			}
			else if (tmp == ',')
			{
				curr = curr->parent;
				curr->rchild = (treenode*)malloc(sizeof(treenode));
				curr->rchild->parent = curr;
				curr = curr->rchild;
				curr->lchild = NULL;
				curr->rchild = NULL;
				CreateList(&(curr->data));
			}
			else if (tmp == ')')
			{
				curr = curr->parent;
			}
			else
			{
				InsertList(&(curr->data), tmp);
			}
		}
		else
			break;
	}
	out_tree(root->lchild);
	out_tree(root->rchild);
	while (root->data._size)
		printf_s("%c", PopList(&(root->data)));
}