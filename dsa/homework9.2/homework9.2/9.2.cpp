#include <stdlib.h>
#include <stdio.h>

struct Stable {
	int* elem;
	int size;
};

int Search(Stable* s, int key, int low, int high)
{
	if (low == high)
	{
		if (s->elem[low] == key)
			return low;
		else
			return 0;
	}
	else
	{
		int mid = (low + high) / 2;
		if (s->elem[mid] == key)
			return mid;
		else if (s->elem[mid] < key)
			return Search(s, key, mid + 1, high);
		else
			return Search(s, key, low, mid);
	}
}

int main()
{
	Stable S;
	scanf_s("%d", &(S.size));
	S.elem = (int*)calloc(S.size + 1, sizeof(int));
	for (int i = 1; i <= S.size - 1; i++)
	{
		scanf_s("%d", &(S.elem[i]));
		scanf_s(" ");
	}
	scanf_s("%d", &(S.elem[S.size]));
	int m;
	scanf_s("%d", &m);
	int* keys = (int*)calloc(m, sizeof(int));
	for (int i = 0; i < m; i++)
		scanf_s("%d", &(keys[i]));
	for (int i = 0; i < m; i++)
		printf("%d\n", Search(&S, keys[i], 1, S.size));
	return 0;
}