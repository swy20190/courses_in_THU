#include <iostream>
#include "List.h"

using namespace std;

int main()
{
	List<int> a=List<int>();
	a.insertAsFirst(2);
	a.insertAsFirst(1);
	a.sort();
	cout << a[0] << a[1];
	return 0;
}