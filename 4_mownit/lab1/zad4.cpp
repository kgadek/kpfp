#include <cstdio>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_ieee_utils.h>

int main() {
	float ff = 1.0;
	double dd = 1.0, ddo;
	ff = ff / 3.0;
	dd = dd / 3.0;

	printf("ff     = ");
	gsl_ieee_printf_float(&ff);
	printf("\t\t\t\t= %+f\t= %+e\t~= 1/3\n", ff, ff);

	printf("dd     = ");
	gsl_ieee_printf_double(&dd);
	printf("\t= %+f\t= %+e\t~= 1/3\n", dd, dd);

	ddo = dd;
	dd = ff;

	printf("dd'    = ");
	gsl_ieee_printf_double(&dd);
	printf("\t= %+f\t= %+e\t~= ff ~=1/3\n", dd, dd);

	dd = ddo - dd;

	printf("dd-dd' = ");
	gsl_ieee_printf_double(&dd);
	printf("\t= %+f\t= %+e\t~= (d)1/3-(f)1/3\n", dd, dd);

	return 0;
}

