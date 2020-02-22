
#include<queue>

#include<cstdio>

#include<cstring>

#include<algorithm>

#define N 10005

using namespace std;

priority_queue<int>q;

int main()

{

	int n, i, x, t, ans = 0;

	scanf_s("%d", &n);

	for (i = 1; i <= n; ++i)

	{

		scanf_s("%d", &x);

		q.push(-x);

	}

	while (--n)

	{

		t = 0;

		t -= q.top(); q.pop();

		t -= q.top(); q.pop();

		ans += t;

		q.push(-t);

	}

	printf_s("%d", ans);

	return 0;

}

