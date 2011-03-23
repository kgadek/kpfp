#include <cstdio>

unsigned long __fib_internal(int n, unsigned long a, unsigned long b) {
	if(n==0)
		return a;
	return __fib_internal(--n, b, a+b);
}

unsigned long fib(int n) {
	return __fib_internal(n, (unsigned long)0, (unsigned long)1);
}

int main() {
	int i,t;
	unsigned long k;
	for(t=0;t<99999;t++) {
		for(i=0; i<49; i++)
			k = fib(i);
	}
	printf("%ld\n",k);
	return 0;
}

