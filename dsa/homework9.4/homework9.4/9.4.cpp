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

char PopList(List* l)//出栈/出队
{
	ListNode* now = l->header->Next;
	char old = now->data;
	now->Next->Last = now->Last;
	l->header->Next = now->Next;
	free(now);
	l->_size--;
	return old;
}


struct treenode {
	treenode* parent = NULL;
	treenode* lchild = NULL;
	treenode* rchild = NULL;
	List data;
	int num;
	bool nul;
};

void out_tree(treenode* T)
{
		if (T->nul)
			printf_s("-");
		else
			printf_s("%d", T->num);
		if (T->lchild != NULL)
		{
			printf_s("(");
			out_tree(T->lchild);
			if (T->rchild == NULL)
				printf_s(")");
		}
		if (T->rchild != NULL)
		{
			if (T->lchild == NULL)
				printf_s("(");
			printf_s(",");
			out_tree(T->rchild);
			printf_s(")");
		}
}

void char_to_int(treenode* T)
{
	if (T)
	{
		int r = 1;
		T->num = 0;
		while (T->data._size)
		{
			T->num = (PopList(&(T->data)) - '0')*r + T->num;
			r = r * 10;
		}
		char_to_int(T->lchild);
		char_to_int(T->rchild);
	}
	else
		return;
}

void clean_tree(treenode* T)
{
	if (T)
	{
		if (T->lchild) {
			if (T->lchild->nul)
				T->lchild = NULL;
		}
		if (T->rchild) {
			if (T->rchild->nul)
				T->rchild = NULL;
		}
		clean_tree(T->lchild);
		clean_tree(T->rchild);
	}
	else
		return;
}

void rebuild_tree(treenode* T)
{
	if (T&&T->nul==0)
	{
		if ((T->lchild == NULL) && (T->rchild))
		{
			T->lchild = (treenode*)malloc(sizeof(treenode));
			T->lchild->parent = T;
			T->lchild->nul = 1;
			T->lchild->lchild = NULL;
			T->lchild->rchild = NULL;
		}
		if ((T->rchild == NULL) && (T->lchild))
		{
			T->rchild = (treenode*)malloc(sizeof(treenode));
			T->rchild->parent = T;
			T->rchild->nul = 1;
			T->rchild->lchild = NULL;
			T->rchild->rchild = NULL;
		}
		rebuild_tree(T->lchild);
		rebuild_tree(T->lchild);
	}
	else
		return;
}

int delVal(treenode** root, int val)
{
	if (root == NULL)
		return 0;
	treenode* p = *root;
	treenode* pre = *root;
	treenode* q = (treenode*)malloc(sizeof(treenode));
	while (1)
	{
		int value = (*root)->num;
		while (p&&p->num >= val)
		{
			pre = p;
			p = p->lchild;
			pre->nul = 1;
			free(pre);
		}
		if (value > val)
			*root = p;
		else
			q->rchild = p;
		while (p&&p->num < val)
		{
			pre = p;
			p = p->rchild;
		}
		if (p == NULL)
			break;
		q = pre;
	}
	return 1;
}

int main()
{
	treenode* root = (treenode*)malloc(sizeof(treenode));
	root->lchild = NULL;
	root->rchild = NULL;
	root->parent = NULL;
	CreateList(&(root->data));
	treenode* curr = root;
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp != '\n')
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
				if (tmp == '-')
					curr->nul = 1;
				else
				{
					curr->nul = 0;
					PushList(&(curr->data), tmp);
				}
			}
		}
		else
			break;
	}
	int key;
	scanf_s("%d", &key);
	char_to_int(root);
	//out_tree(root);
	clean_tree(root);
	out_tree(root);
	printf("\n");
	delVal(&root, key);
	//out_tree(root);
	rebuild_tree(root);
	if(root)
		out_tree(root);
	
}