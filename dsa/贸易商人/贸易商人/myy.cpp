#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
	int n, m;
	cin >> n >> m;
	int** sq = new int*[n];
	for (int i = 0; i < n; i++)
		sq[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			sq[i][j] = 0;
	int start, end;
	for (int i = 0; i < m; i++)
	{
		cin >> start >> end;
		sq[start][end] = sq[end][start] = 1;
	}

}