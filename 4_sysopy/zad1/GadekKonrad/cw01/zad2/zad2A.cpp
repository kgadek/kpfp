#include <cstdio>
#include <cstdlib>

unsigned long a(int m, int n) {
	if(m && n)
		return a(m-1, a(m,n-1));
	else if(m)
		return a(m-1,1);
	else
		return n+1;
}

int main() {
	printf("%ld\n", a(4,1));
	return 0;
}

