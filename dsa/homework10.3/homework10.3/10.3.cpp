#include <stdio.h>
#include <stdlib.h>

void adjust(int* tritree, int tsize,int root)
{
	int Min = tritree[root];
	int parent = root;
	int left = 3 * parent + 1;
	while (left < tsize)
	{
		if (left + 2 < tsize)//3 sons
		{
			int min = Min;
			int key = -1;
			for (int i = 0; i < 3; i++)
			{
				if (min > tritree[left + i])
				{
					min = tritree[left + i];
					key = left + i;
				}
			}
			if (key == -1)
				break;
			else
			{
				int tmp = tritree[key];
				tritree[key] = tritree[parent];
				tritree[parent] = tmp;
				printf_s("%d", tmp);
				printf_s(" ");
				parent = key;
				left = 3 * parent + 1;
			}
		}
		else if (left + 1 < tsize)//2 sons
		{
			int key = -1;
			int min = Min;
			for (int i = 0; i < 2; i++)
			{
				if (min > tritree[left + i])
				{
					min = tritree[left + i];
					key = left + i;
				}
			}
			if (key == -1)
				break;
			else
			{
				int tmp = tritree[key];
				tritree[key] = tritree[parent];
				tritree[parent] = tmp;
				printf_s("%d", tmp);
				printf_s(" ");
				parent = key;
				left = 3 * parent + 1;
			}
		}
		else//1 son
		{
			int min = Min;
			if (min < tritree[left])
				break;
			else
			{
				tritree[parent] = tritree[left];
				tritree[left] = min;
				printf_s("%d", tritree[parent]);
				printf_s(" ");
				parent = left;
				left = 3 * parent + 1;
			}
		}
	}
}

int main()
{
	int n;
	scanf_s("%d", &n);
	int* arry = (int*)calloc(n, sizeof(int));
	for (int i = 0; i < n; i++)
		scanf_s("%d", &arry[i]);
	int size = n;
	for (int i = 0; i < n; i++)
	{
		printf_s("%d", arry[0]);
		printf_s(" ");
		if (size != 1)
		{
			arry[0] = arry[size - 1];
			size--;
			adjust(arry, size, 0);
			printf_s("%d", arry[size]);
			printf_s("\n");
		}
	}
}