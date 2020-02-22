#include <stdio.h>
#include <stdlib.h>
//A psyeudo-hash program
int main()
{
	int n;
	scanf_s("%d", &n);
	char** hash = (char**)calloc(n, sizeof(char));
	for (int i = 0; i < n; i++)
		hash[i] = (char*)calloc(105, sizeof(char));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < 105; j++)
			hash[i][j] = 'a';
	char fck;
	scanf_s("%c", &fck);
	for (int i = 0; i < n; i++)
	{
		int curr = 0;
		char tmp;
		while (1)
		{
			scanf_s("%c", &tmp);
			if (tmp == '\n')
				break;
			hash[i][curr] = tmp;
			curr++;
		}

	}

	for (int i = 0; i < 26; i++)
	{
		char begin = 'A' + i;
		for (int k = 0; k < n; k++)
		{
			if (hash[k][0] == begin)
			{
				int pt = 0;
				while (1)
				{
					if (hash[k][pt] == 'a')
						break;
					printf_s("%c", hash[k][pt]);
					pt++;
				}
				printf_s("\n");
			}
		}
	}
}