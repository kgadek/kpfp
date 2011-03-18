#include <stdio.h>
#include <unistd.h>

int main() {
	unsigned long a, b, i;
	a = 0;
	b = 1;
	i = 0;
	while(b > 0) {
		b = a + b;
		a = b - a;
		printf("F(%ld) = %ld\n", ++i, b);
		sleep(1);
	}
	return 0;
}

