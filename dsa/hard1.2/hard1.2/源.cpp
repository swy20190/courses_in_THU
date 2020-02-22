#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
	int N, L, R;
	cin >> N >> L >> R;
	int* list = new int[N];
	for (int i = 0; i < N; i++)
		cin >> list[i];
	float max = 0.0;
	for (int l = L; l <= R; l++)
	{
		for (int start = 0; start <= N - L; start++)
		{
			int sum = 0;
			for (int i = start; i < start + l; i++)
				sum += list[i];
			float tmp = (float)sum / float(l);
			if (max < tmp)
				max = tmp;
		}
	}
	cout << (int)(max+0.5);
}