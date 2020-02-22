#include <stdlib.h>
#include <stdio.h>

void Search(int* l, int size, int* p,int key)
{
	int times = 0;
	while (1)
	{
		times++;
		if (l[*p] == key)
		{
			printf("1");
			printf(" ");
			printf("%d\n", times);
			break;
		}
		else if (l[*p] < key)
		{
			int next = (*p + 1) % size;
			if (next == 0 )
			{
				*p = next;
				printf("0");
				printf(" ");
				printf("%d\n", times);
				break;
			}
			else if (l[next] > key)
			{
				*p = next;
				printf("0");
				printf(" ");
				printf("%d\n", times + 1);
				break;
			}
			else
				*p = next;
		}
		else
		{
			int next = (*p - 1+size) % size;
			if (next == size - 1 )
			{
				*p = next;
				printf("0");
				printf(" ");
				printf("%d\n", times);
				break;
			}
			else if (l[next] < key)
			{
				*p = next;
				printf("0");
				printf(" ");
				printf("%d\n", times + 1);
				break;
			}
			else
				*p = next;
		}
	}
}

int main()
{
	int n, m;
	scanf_s("%d", &n);
	scanf_s(" ");
	scanf_s("%d", &m);
	int* list = (int*)calloc(n, sizeof(int));
	for (int i = 0; i < n - 1; i++)
	{
		scanf_s("%d", &(list[i]));
		scanf_s(" ");
	}
	scanf_s("%d", &(list[n - 1]));
	int* keys = (int*)calloc(m, sizeof(int));
	for (int i = 0; i < m - 1; i++)
	{
		scanf_s("%d", &(keys[i]));
		scanf_s(" ");
	}
	scanf_s("%d", &(keys[m - 1]));
	int pointer = 0;
	for (int i = 0; i < m; i++)
		Search(list, n, &pointer, keys[i]);
	return 0;
}