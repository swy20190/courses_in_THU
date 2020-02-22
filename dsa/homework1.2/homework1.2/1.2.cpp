#include <stdio.h>
#include <stdlib.h>
int main()
{
	int n1;
	char *array;
	//printf("请输入所要创建的一维动态数组的长度：");
	scanf_s("%d",&n1);
	array=(char*)calloc(n1,sizeof(char));
	for(int i=0;i<n1;i++)
	{ 
		scanf_s("%c", &array[i]);
	}
	for(int i=n1-1;i>=0;i--)
	{ 
		printf("%c", array[i]);
	} 
	free(array);//释放第一维指针
	return 0;
}
