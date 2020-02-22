#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	int row;
	int col;
	int key;
}trident;

int main()
{
	int n;
	scanf_s("%d", &n);
	trident* array = (trident*)calloc(n, sizeof(trident));
	for (int i = 0; i < n; i++)
	{
		scanf_s("%d", &array[i].row);
		scanf_s("%d", &array[i].col);
		scanf_s("%d", &array[i].key);
	}
	int m;
	scanf_s("%d", &m);
	for (int i = 0; i < m; i++)
	{
		int qrow;
		int qcol;
		int flag = 0;
		scanf_s("%d", &qrow);
		scanf_s("%d", &qcol);
		for (int j = 0; j < n; j++)
		{
			if (array[j].row == qrow&&array[j].col == qcol)
			{
				flag = 1;
				printf_s("%d\n", array[j].key);
				break;
			}
		}
		if (flag == 0)
			printf_s("0\n");
	}
}