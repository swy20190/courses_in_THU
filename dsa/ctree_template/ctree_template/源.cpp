#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef int datatype;
typedef struct TreeNode 
{
	datatype elem;
	struct TreeNode* parent;
	struct TreeNode* lchild;
	struct TreeNode* rchild;
}TreeNode;

typedef struct BinTree
{
	int _size;
	TreeNode* _root;
}BinTree;

TreeNode* CreateNode(datatype e)
{
	TreeNode* np = (TreeNode*)malloc(sizeof(TreeNode));
	np->elem = e;
	np->parent = NULL;
	np->rchild = NULL;
	np->lchild = NULL;
	return np;
}

TreeNode* insertAsLC(TreeNode* dst, datatype src)
{
	TreeNode* np = CreateNode(src);
	dst->lchild = np;
	np->parent = dst;
	return np;
}

TreeNode* insertAsRC(TreeNode* dst, datatype src)
{
	TreeNode* np = CreateNode(src);
	dst->rchild = np;
	np->parent = dst;
	return np;
}

//利用先序遍历序列创建二叉树，序列中null_node代表空节点
TreeNode* _CreateTree(datatype array[], int size, datatype null_node, int* index)
{
	if (*index >= size)
		return NULL;
	if (array[*index] == null_node)
	{
		(*index)++;
		return NULL;
	}
	TreeNode* newnode = CreateNode(array[(*index)++]);
	newnode->lchild = _CreateTree(array, size, null_node, index);
	newnode->rchild = _CreateTree(array, size, null_node, index);
	return newnode;
}

TreeNode* CreateTree(datatype array[], int size, datatype null_node)
{
	int index = 0;
	return _CreateTree(array, size, null_node, &index);
}

//求二叉树高度
int TreeHeight(TreeNode* root)
{
	if (root == NULL)
		return 0;
	int lh = TreeHeight(root->lchild)+1;
	int rh = TreeHeight(root->rchild)+1;
	return lh > rh ? lh : rh;
}

//求镜像
void Mirror(TreeNode* root)
{
	if (root == NULL)
		return;
	TreeNode* tmp = root->lchild;
	root->lchild = root->rchild;
	root->rchild = tmp;
	Mirror(root->lchild);
	Mirror(root->rchild);
	return;
}

int main()
{
	return 0;
}
