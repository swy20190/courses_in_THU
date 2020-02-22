#include <stdio.h>
#include <stdlib.h>
int main()
{
	int n1,n2;
	int *array1;
	int *array2;
	//printf("请输入所要创建的一维动态数组的长度：");
	scanf_s("%d",&n1);
	array1=(int*)calloc(n1,sizeof(int));
	for(int i=0;i<n1;i++)
	{ 
		scanf_s("%d", &array1[i]);
	} 
	scanf_s("%d", &n2);
	array2 = (int*)calloc(n2, sizeof(int));
	for (int i = 0; i < n2; i++)
	{
		scanf_s("%d", &array2[i]);
	}
	if (n1 < n2)
	{
		int flag = 1;
		int range = n1;
		for (int i = 0; i < range; i++)
		{
			if (array1[i] < array2[i])
			{
				printf("-1");
				flag = 0;
				break;
			}
			else if (array1[i] > array2[i])
			{
				printf("1");
				flag = 0;
				break;
			}
		}
		if (flag)
			printf("-1");
	}
	else if (n1 > n2)
	{
		int flag = 1;
		int range = n2;
		for (int i = 0; i < range; i++)
		{
			if (array1[i] < array2[i])
			{
				printf("-1");
				flag = 0;
				break;
			}
			else if (array1[i] > array2[i])
			{
				printf("1");
				flag = 0;
				break;
			}
		}
		if (flag)
			printf("1");
	}
	else
	{
		int flag = 1;
		int range = n1;
		for (int i = 0; i < range; i++)
		{
			if (array1[i] < array2[i])
			{
				printf("-1");
				flag = 0;
				break;
			}
			else if (array1[i] > array2[i])
			{
				printf("1");
				flag = 0;
				break;
			}
		}
		if (flag)
			printf("0");
	}
	free(array1);//释放第一维指针 
	free(array2);
    return 0;
}
