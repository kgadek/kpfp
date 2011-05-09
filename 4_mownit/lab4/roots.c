/*Sepultura - Roots Bloody Roots!*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

typedef unsigned int uint;



struct f_wielomianowa_params {				/* ((...(a[n] x + a[n-1]) x + a[n-2]) x + ...) + a[0] */
	uint n;												/*stopień wielomianu*/
	double *a;											/*współczynniki (tablica n+1 liczb)*/
};
double f_wielomianowa_def(double,void*);				/*wartość funkcji*/
double f_wielomianowa_df(double,void*);					/*wartość pochodnej funkcji*/
void f_wielomianowa_fdf(double,void*,double*,double*);	/*obie wartości naraz*/

void roots_f(gsl_function*, const gsl_root_fsolver_type*, double, double, double, double, uint);



int main(/*int argc, char **argv*/) {
	gsl_function f_kwadratowa_f;
	gsl_function_fdf f_kwadratowa_fdf;
	struct f_wielomianowa_params f_kwadratowa_pInst;
	double *pdtmp;

	f_kwadratowa_pInst.n = 2;										/*funkcja kwadratowa*/
	pdtmp = (double*)malloc((f_kwadratowa_pInst.n+1)*sizeof(double));
	pdtmp[2]=1.0;					/*x^2+2x*/
	pdtmp[1]=2.0;
	pdtmp[0]=0.0;
	f_kwadratowa_pInst.a = pdtmp;
	f_kwadratowa_f.function = f_wielomianowa_def;					/*funkcja kwadratowa (bracketing)*/
	f_kwadratowa_f.params = &f_kwadratowa_pInst;
	f_kwadratowa_fdf.f = f_wielomianowa_def;						/*funkcja kwadratowa (polishing)*/
	f_kwadratowa_fdf.df = f_wielomianowa_df;
	f_kwadratowa_fdf.fdf = f_wielomianowa_fdf;
	f_kwadratowa_fdf.params = &f_kwadratowa_pInst;

																	/*szukanie pierwiastka*/
	roots_f(&f_kwadratowa_f, gsl_root_fsolver_brent, 5, -1.0, 10.0, 0, 100);

	free(f_kwadratowa_pInst.a);										/*końcówka*/
	f_kwadratowa_pInst.a = 0;
	return 0;
}


double f_wielomianowa_def(double x,void *params) {
	uint i;
	double res = 0.0;
	struct f_wielomianowa_params *p = (struct f_wielomianowa_params*)params;
	for(i=p->n; i>0; --i)
		res = res*x + p->a[i];
	res = res*x + p->a[0];
	return res;
}
double f_wielomianowa_df(double x,void *params) {
	uint i;
	double res = 0.0;
	struct f_wielomianowa_params *p = (struct f_wielomianowa_params*)params;
	for(i=p->n; i>0; --i)
		res = res*x + p->a[i]*(double)i;
	return res;
}
void f_wielomianowa_fdf(double x,void *params,double *f,double *df) {
	*f = f_wielomianowa_def(x,params);		/*nie ma chyba prostej metody na przyspieszenie obliczeń*/
	*df = f_wielomianowa_df(x,params);
}
void roots_f(gsl_function *fun, const gsl_root_fsolver_type *T,
		double r, double x_lo, double x_hi, double r_exp, uint maxit) {
	uint it = 0;							/*iteracja*/
	int status;								/*status solvera*/
	gsl_root_fsolver *s;					/*solver*/

	s = gsl_root_fsolver_alloc(T);
	gsl_root_fsolver_set(s, fun, x_lo, x_hi);

	printf("[bracketing: %s]\n", gsl_root_fsolver_name (s));
	printf ("%5s [%11s, %11s] %11s %11s %11s\n",
			"iter", "lower", "upper", "root", "err", "err(est)");
    do{
        ++it;
        status = gsl_root_fsolver_iterate(s);
        r = gsl_root_fsolver_root(s);
        x_lo = gsl_root_fsolver_x_lower(s);
        x_hi = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval(x_lo, x_hi, 0.001, 0.001);
        if(status == GSL_SUCCESS)
            printf("ROOTS! BLOODY ROOTS!\n");

        printf("%5d [%11.7f, %11.7f] %11.7f %11.7f %11.7f\n",
        		it, x_lo, x_hi, r, r - r_exp, x_hi - x_lo);
    } while(status == GSL_CONTINUE && it < maxit);

    gsl_root_fsolver_free(s);
}
