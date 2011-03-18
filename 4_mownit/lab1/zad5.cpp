#include <cstdio>
#include <typeinfo>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_ieee_utils.h>

template <class T>
T mySqrt(T S) {
	T x, xn = 1.0;
	do {
		x = xn;
		xn = ((T)((T)(S / x) + x)) * (T)0.5;
	} while(x != xn);
	return xn;
}

template <class T>
T myPow(T x, int n) {
	T r = 1.0;
	while(n--)
		r *= x;
	return r;
}

template <class T>
T fib_Binet(int n) {
	T sqf = mySqrt((T)5.0);
	T phi = ((T)(sqf + (T)1.0)) * (T)0.5;
	T phii = -phi + (T)1.0;
	T r = ((T)(myPow<T>(phi, n) - myPow(phii, n))) / sqf;
	return r;
}

template <class T>
void test(int n) {
	T fB, fEt, fD;
	unsigned long long fE = 0, fpE = 1;
	for(int i = 0; i < n; ++i) {
		fEt = fE;
		fB = fib_Binet<T>(i);
		fD = fEt - fB;
		printf("F(%d) = %lld ~= [%s] %f \tdiff: %f\n", i, fE, typeid(fB).name(), fB, fD);
		fE += fpE;
		fpE = fE - fpE;
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2)
		return 1;

	test<double>(atoi(argv[1]));
	test<float>(atoi(argv[1]));

	return 0;
}

