#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>



struct treenode {
	struct treenode* parent ;
	struct treenode* lchild ;
	struct treenode* rchild ;
	char data;
};


int main()
{
	struct treenode* root = (struct treenode*)malloc(sizeof(struct treenode));
	struct treenode* curr = root;
	int node_num = 0;
	while (1)
	{
		char tmp;
		scanf_s("%c", &tmp);
		if (tmp != '\n')
		{
			if (tmp == '(')
			{
				curr->lchild = (struct treenode*)malloc(sizeof(struct treenode));
				curr->lchild->parent = curr;
				curr = curr->lchild;
				curr->lchild = NULL;
				curr->rchild = NULL;
				curr->data = 'a';
			}
			else if (tmp == ',')
			{
				curr = curr->parent;
				curr->rchild = (struct treenode*)malloc(sizeof(struct treenode));
				curr->rchild->parent = curr;
				curr = curr->rchild;
				curr->lchild = NULL;
				curr->rchild = NULL;
				curr->data = 'a';
			}
			else if (tmp == ')')
			{
				curr = curr->parent;
			}
			else
			{
				curr->data = tmp;
				node_num++;
			}
		}
		else
			break;
	}
	//struct List q;
	//CreateList(&q);
	//enQ(&q, root);
	struct treenode** q = (struct treenode**)calloc(node_num + 5, sizeof(struct treenode*));
	int head = 0;
	int tail = 1;
	q[0] = root;
	while (head<tail)
	{
		struct treenode* now = q[head];
		head++;
		if (now->data != 'a')
		{
			printf_s("%c", now->data);
			printf_s(" ");
		}
		if (now->lchild)
		{
			q[tail] = now->lchild;
			tail++;
		}
		if (now->rchild)
		{
			q[tail] = now->rchild;
			tail++;
		}
	}
}