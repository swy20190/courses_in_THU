#include <stdio.h>
#include <stdlib.h>

int find_son(int u0, int v0, int* L, int* R)//判断u是否是v的子孙
{
	if (u0 == v0)
		return 1;
	else if (v0 == 0)
		return 0;
	else
	{
		return find_son(u0, L[v0], L, R) || find_son(u0, R[v0], L, R);
	}
}

int main()
{
	int u, v;
	int n;
	scanf_s("%d", &n);
	int* L;
	int* R;
	L = (int*)calloc(n+1, sizeof(int));
	R = (int*)calloc(n+1, sizeof(int));
	for (int i = 0; i <= n; i++)
		scanf_s("%d", &L[i]);
	for (int i = 0; i <= n; i++)
		scanf_s("%d", &R[i]);
	scanf_s("%d", &u);
	scanf_s("%d", &v);
	int flag = find_son(u, v, L, R);
	if (flag)
		printf_s("true");
	else
		printf_s("false");
	return 0;
}