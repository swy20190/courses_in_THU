#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct treenode {
	treenode* parent;
	treenode* lchild;
	treenode* rchild;
	char data;
};

struct ListNode {
	treenode* data;
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

void InsertList(List* l, treenode* e)//空表，在尾节点前插入
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	l->trailer->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}


treenode* PopList(List* l)//出栈/出队
{
	ListNode* now = l->header->Next;
	treenode* old = now->data;
	now->Next->Last = now->Last;
	l->header->Next = now->Next;
	free(now);
	l->_size--;
	return old;
}

void enQ(List* l, treenode* e)//入队
{
	ListNode* np = (ListNode*)malloc(sizeof(ListNode));
	np->data = e;
	np->Next = l->trailer;
	np->Last = l->trailer->Last;
	np->Last->Next = np;
	l->trailer->Last = np;
	l->_size++;
}

void clean(List* l)//清空
{
	while (l->_size)
		PopList(l);
	free(l->header);
	free(l->trailer);
}

treenode* find(treenode* r, char key)//查找节点
{
	List q;
	CreateList(&q);
	if (r != nullptr)
	{
		enQ(&q, r);
		while (q._size)
		{
			treenode* curr = PopList(&q);
			if (curr->data == key)
			{
				return curr;
				break;
			}
			else
			{
				if (curr->lchild != nullptr)
					enQ(&q, curr->lchild);
				if (curr->rchild != nullptr)
					enQ(&q, curr->rchild);
			}
		}
	}
	clean(&q);
}

void printtree(treenode* r)
{
	printf_s("%c", r->data);
	if (r->lchild != nullptr)
	{
		printf_s("(");
		printtree(r->lchild);
		if (r->rchild == nullptr)
			printf_s(")");
	}
	if (r->rchild!=nullptr)
	{
		if (r->lchild == nullptr)
			printf_s("(");
		printf_s(",");
		printtree(r->rchild);
		printf_s(")");
	}
}

int main()
{
	treenode* root = (treenode*)malloc(sizeof(treenode));
	root->lchild = NULL;
	root->rchild = NULL;
	root->parent = NULL;
	treenode* curr = root;
	while (1)
	{
		char tmp[4];
		for (int i = 0; i < 4; i++)
			scanf_s("%c", &tmp[i]);
		if (tmp[0] == '^'&&tmp[1] == '^')
			break;
		else
		{
			if (tmp[0] == '^')
			{
				root->data = tmp[1];
			}
			else
			{
				curr = find(root, tmp[0]);
				if (tmp[2] == 'L')//lc
				{
					curr->lchild = (treenode*)malloc(sizeof(treenode));
					curr->lchild->data = tmp[1];
					curr->lchild->parent = curr;
					curr->lchild->lchild = NULL;
					curr->lchild->rchild = NULL;
				}
				else
				{
					curr->rchild = (treenode*)malloc(sizeof(treenode));
					curr->rchild->data = tmp[1];
					curr->rchild->parent = curr;
					curr->rchild->lchild = NULL;
					curr->rchild->rchild = NULL;
				}
			}
		}

	}
	printtree(root);
}