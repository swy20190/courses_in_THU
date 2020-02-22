#include <stdio.h>
#include <stdlib.h>

//a fake qsort
int main()
{
	int n;
	scanf_s("%d", &n);
	int* arry = (int*)calloc(n, sizeof(int));
	for (int i = 0; i < n; i++)
		scanf_s("%d", &arry[i]);
	int tmp;
	for (int i = 0; i < n - 1; i++)
	{
		for(int j=n-1;j>i;j--)
			if (arry[j] < arry[j - 1])
			{
				tmp = arry[j];
				arry[j] = arry[j - 1];
				arry[j - 1] = tmp;
			}
	}
	for (int i = 0; i < n; i++)
	{
		printf_s("%d", arry[i]);
		printf_s(" ");
	}
}