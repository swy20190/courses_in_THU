#include <stdio.h>
#include <stdlib.h>
int main()
{
	int n1,n2;
	int *array1;
	int *array2;
	int *array3;
	//printf("请输入所要创建的一维动态数组的长度：");
	scanf_s("%d", &n1);
	array1 = (int*)calloc(n1, sizeof(int));
	for (int i = 0; i<n1; i++)
	{
		scanf_s("%d", &array1[i]);
	}
	scanf_s("%d", &n2);
	array2 = (int*)calloc(n2, sizeof(int));
	for (int i = 0; i < n2; i++)
	{
		scanf_s("%d", &array2[i]);
	}
	int ma;
	if (n1 > n2)
		ma = n1;
	else
		ma = n2;
	array3 = (int*)calloc(ma, sizeof(int));
	int pt = 0;
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			if (array1[i] == array2[j])
			{
				array3[pt] = array1[i];
				pt++;
			}
			else if (array1[i] < array2[j])
				break;
		}
	}
	printf("%d", pt);
	printf("\n");
	for (int i = 0; i < pt; i++)
	{
		printf("%d\t", array3[i]);
	}
	free(array1);//释放第一维指针
	free(array2);
	free(array3);
	return 0;
}