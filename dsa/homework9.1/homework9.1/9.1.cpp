#include <stdlib.h>
#include <stdio.h>

struct stable {
	int* elem;
	int size;
};

int Search(stable* s, int key,int low,int high)
{
	if (low + 1 == high)
		return low;
	else {
		int mid = (low + high) / 2;
		if (s->elem[mid] == key)
			return mid;
		else if (s->elem[mid] < key)
			return Search(s, key, mid, high);
		else
			return Search(s, key, low, mid);
	}
}

int main()
{
	stable S;
	scanf_s("%d", &S.size);
	S.elem = (int*)calloc(S.size + 2, sizeof(int));
	for (int i = 1; i <= S.size - 1; i++)
	{
		scanf_s("%d", &(S.elem[i]));
		scanf_s(" ");
	}
	scanf_s("%d", &(S.elem[S.size]));
	S.elem[0] = -1;
	S.elem[S.size + 1] = 100000005;
	int m;
	scanf_s("%d", &m);
	int* keys = (int*)calloc(m, sizeof(int));
	for (int i = 0; i < m - 1; i++)
	{
		scanf_s("%d", &(keys[i]));
		scanf_s(" ");
	}
	scanf_s("%d", &(keys[m - 1]));
	for (int i = 0; i < m; i++)
		printf_s("%d\n", Search(&S, keys[i], 0, S.size + 1));
	return 0;
}