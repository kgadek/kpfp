#include <cstdio>
#include <typeinfo>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_ieee_utils.h>

template <class T>
T min_num(T start, void (*pF)(const T*)) {
	T x = start, two = 2, p;
	printf("::::::::::::::::::::[min_num %s]::::::::::::::::::::\n", typeid(x).name());
	p = x;
	while(x > (T)0.0) {
		p = x;
		x = x / two;
		printf("\tx = ");
		pF(&x);
		printf("\t\t= %e\n", x);
	}
	return p;
}

int main() {
	float ff = min_num<float>(23, &gsl_ieee_printf_float);
	double dd = min_num<double>(23, &gsl_ieee_printf_double);
	printf("min num\t\tF %e\t\tD %e\n", ff, dd);
	return 0;
}

