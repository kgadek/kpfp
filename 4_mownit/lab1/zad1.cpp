#include <cstdio>
#include <cstdlib>

template <class T>
T f(int max) {
	T xn, tmp, tmpp, tmppp;
	tmp = tmpp = tmppp = 0.0;
	xn = (T)0.01;
	while(max--) {
		tmp = 1 - xn;
		tmpp = tmp * xn;
		tmppp = tmpp * (T)3.0;
		xn = (T)(xn + tmppp);
	}
	return xn;
}

int main(int argc, char **argv) {
	if(argc < 2) return 1;
	int Nsize = atoi(argv[1]);
	for(int N = 1; N <= Nsize; ++N)
		printf("%d\t\tF %f\t\tD %lf\t\tLD %Lf\n", N,
				f<float>(N), f<double>(N), f<long double>(N));
	return 0;
}

