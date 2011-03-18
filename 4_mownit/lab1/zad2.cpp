#include <cstdio>

template <class T>
T eps() {
	T x = 1, one = 1, two = 2, p;
	p = x;
	while((T)(x + one) > one) {
		p = x;
		x = x / two;
	}
	return p;
}

int main() {
	printf("eps:\t\tF %e\t\tD %e\t\tLD %Le\n",
			eps<float>(), eps<double>(), eps<long double>());
	return 0;
}

