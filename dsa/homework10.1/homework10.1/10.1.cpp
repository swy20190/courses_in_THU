#include <stdlib.h>
#include <stdio.h>

int main()
{
	int n;
	scanf_s("%d", &n);
	int* arry = (int*)calloc(n + 1, sizeof(int));
	for (int i = 1; i <= n; i++)
		scanf_s("%d", &arry[i]);
	int flag1 = 0;
	int flag2 = 0;
	int tmp = 0;
	if (n % 2 == 1)//项数为奇数
	{
		while (1)
		{
			flag2 = flag1;
			flag1 = 1;
			for (int i = 1; i <= n - 2; i = i + 2)
			{
				if (arry[i] > arry[i + 1])
				{
					tmp = arry[i + 1];
					arry[i + 1] = arry[i];
					arry[i] = tmp;
					flag1 = 0;
				}
			}
			for (int i = 1; i <= n; i++)
			{
				printf_s("%d", arry[i]);
				printf_s(" ");
			}
			printf_s("\n");
			if (flag1 == 1 && flag2 == 1)
				break;
			flag2 = flag1;
			flag1 = 1;
			for (int i = 2; i <= n - 1; i = i + 2)
			{
				if (arry[i] > arry[i + 1])
				{
					tmp = arry[i + 1];
					arry[i + 1] = arry[i];
					arry[i] = tmp;
					flag1 = 0;
				}
			}
			for (int i = 1; i <= n; i++)
			{
				printf_s("%d", arry[i]);
				printf_s(" ");
			}
			printf_s("\n");
			if (flag1 == 1 && flag2 == 1)
				break;
		}
	}
	else //项数为偶数
	{
		while (1)
		{
			flag2 = flag1;
			flag1 = 1;
			for (int i = 1; i <= n - 1; i=i+2)
			{
				if (arry[i] > arry[i + 1])
				{
					tmp = arry[i + 1];
					arry[i + 1] = arry[i];
					arry[i] = tmp;
					flag1 = 0;
				}
			}
			for (int i = 1; i <= n; i++)
			{
				printf_s("%d", arry[i]);
				printf_s(" ");
			}
			printf_s("\n");
			if (flag1 == 1 && flag2 == 1)
				break;
			flag2 = flag1;
			flag1 = 1;
			for (int i = 2; i <= n - 2; i=i+2)
			{
				if (arry[i] > arry[i + 1])
				{
					tmp = arry[i + 1];
					arry[i + 1] = arry[i];
					arry[i] = tmp;
					flag1 = 0;
				}
			}
			for (int i = 1; i <= n; i++)
			{
				printf_s("%d", arry[i]);
				printf_s(" ");
			}
			printf_s("\n");
			if (flag1 == 1 && flag2 == 1)
				break;
		}
	}
}