#include <stdio.h>
#include <stdlib.h>

int main()
{
	int k;
	int max;
	scanf_s("%d", &k);
	scanf_s("%d", &max);
	int* queue;
	queue = (int*)calloc(k, sizeof(int));
	for (int i = 0; i < k; i++)
		queue[i] = 0;
	queue[0] = 1;
	int rank = 1;
	while (queue[(rank - 1) % k] <= max)
	{
		printf_s("%d", queue[(rank-1)%k]);
		printf_s(" ");
		int sum = 0;
		for (int i = 0; i < k; i++)
			sum += queue[i];
		rank++;
		queue[(rank - 1) % k] = sum;
	}
}