/*Sepultura - Roots Bloody Roots!*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include "f_wielomianowa.h"
#include "f_sinxmione.h"

typedef unsigned int uint;


void roots_f(gsl_function*, const gsl_root_fsolver_type*, double, double, double, double, uint);
void roots_fdf(gsl_function_fdf*, const gsl_root_fdfsolver_type*,double, double, uint);

const char *lyrics[] = {
		"Roots bloody roots",
		"Roots bloody roots",
		"Roots bloody roots",
		"Roots bloody",
		"",
		"I believe in our fate",
		"We don't need to fake",
		"It's all we wanna be ",
		"Watch me freak",
		"",
		"I say we're growing every day",
		"Getting stronger in every way",
		"I'll take you to a place where",
		"            we shall find our",
		"Roots bloody roots",
		"Roots bloody roots",
		"Roots bloody roots",
		"Roots bloody roots",
		"Rain bring me the strength to",
		"           get to another day",
		"And all I want to see ",
		"Set us free",
		"Why can't you see?",
		"Can't you feel this is real?",
		"",
		"I pray we don't need to change",
		"          our ways to be saved",
		"That all we wanna be ",
		"Watch us freak"};
uint lyrics_s = sizeof(lyrics)/sizeof(const char*);


int main(/*int argc, char **argv*/) {
	gsl_function f_kwadratowa_f;
	gsl_function_fdf f_kwadratowa_fdf;
	struct f_wielomianowa_params f_kwadratowa_pInst;

	gsl_function f_sinxmione_f;
	gsl_function_fdf f_sinxmione_fdf;

	double *pdtmp;

	f_kwadratowa_pInst.n = 2;										/*instancja: funkcja kwadratowa*/
	pdtmp = (double*)malloc((f_kwadratowa_pInst.n+1)*sizeof(double));
	pdtmp[2]=1.0;					/*x^2+2x*/
	pdtmp[1]=2.0;
	pdtmp[0]=0.0;
	f_kwadratowa_pInst.a = pdtmp;									/*definicja: funkcja kwadratowa (bracketing)*/
	f_kwadratowa_fdf.f = f_kwadratowa_f.function = &f_wielomianowa_def;
	f_kwadratowa_f.params = f_kwadratowa_fdf.params = &f_kwadratowa_pInst;
	f_kwadratowa_fdf.df = &f_wielomianowa_df;						/*definicja: funkcja kwadratowa (polishing)*/
	f_kwadratowa_fdf.fdf = &f_wielomianowa_f_df;
	f_sinxmione_f.function = f_sinxmione_fdf.f = &f_sinxmione_def;	/*definicja: funkcja 1/sin(x)-1 (bracketing)*/
	f_sinxmione_f.params = f_sinxmione_fdf.params = NULL;
	f_sinxmione_fdf.df = &f_sinxmione_df;							/*definicja: funkcja 1/sin(x)-1 (polishing)*/
	f_sinxmione_fdf.fdf = & f_sinxmione_f_df;

																	/*szukanie pierwiastków*/
	printf("==========[ x^2 + 2x = 0 ]==========================================\n");
	roots_f(&f_kwadratowa_f, gsl_root_fsolver_brent, 5, -1.0, 10.0, 0, 100);
	printf("\n\n");
	roots_f(&f_kwadratowa_f, gsl_root_fsolver_bisection, 5, -1.0, 10.0, 0, 100);
	printf("\n\n");
	roots_f(&f_kwadratowa_f, gsl_root_fsolver_falsepos, 5, -1.0, 10.0, 0, 100);
	printf("\n\n");

	roots_fdf(&f_kwadratowa_fdf, gsl_root_fdfsolver_newton, 20.0, 0.0, 100);
	printf("\n\n");
	roots_fdf(&f_kwadratowa_fdf, gsl_root_fdfsolver_steffenson, 20.0, 0.0, 100);
	printf("\n\n\n\n");
	/*roots_fdf(&f_kwadratowa_fdf, gsl_root_fdfsolver_secant, 20.0, 0.0, 100);*/	/*psuje się*/

	printf("==========[ 1/sin(x) - 1 = 0 ]======================================\n");
	/*roots_f(&f_sinxmione_f, gsl_root_fsolver_brent, 1, 0.1, 3.13, 1.570796326794896619231321691, 100);
	printf("\n\n");
	roots_f(&f_sinxmione_f, gsl_root_fsolver_bisection, 1, 0.1, 3.13, 1.570796326794896619231321691, 100);
	printf("\n\n");
	roots_f(&f_sinxmione_f, gsl_root_fsolver_falsepos, 1, 0.1, 3.13, 1.570796326794896619231321691, 100);
	printf("\n\n");*/

	roots_fdf(&f_sinxmione_fdf, gsl_root_fdfsolver_newton, 3.13, 1.570796326794896619231321691, 100);
	printf("\n\n");
	roots_fdf(&f_sinxmione_fdf, gsl_root_fdfsolver_steffenson, 3.13, 1.570796326794896619231321691, 100);
	printf("\n\n");
	roots_fdf(&f_sinxmione_fdf, gsl_root_fdfsolver_secant, 3.13, 1.570796326794896619231321691, 100);

	free(f_kwadratowa_pInst.a);										/*końcówka*/
	f_kwadratowa_pInst.a = 0;
	return 0;
}

void roots_f(gsl_function *fun, const gsl_root_fsolver_type *T,
		double r, double x_lo, double x_hi, double r_exp, uint maxit) {
	uint it = 0;							/*iteracja*/
	int status;								/*status solvera*/
	gsl_root_fsolver *s;					/*solver*/

	s = gsl_root_fsolver_alloc(T);
	gsl_root_fsolver_set(s, fun, x_lo, x_hi);

	printf("[bracketing: %s]\n", gsl_root_fsolver_name (s));
	printf("%5s [%11s, %11s] %11s %11s %11s\n",
			"iter", "lower", "upper", "root", "err", "err(est)");
    do{
        ++it;
        status = gsl_root_fsolver_iterate(s);
        r = gsl_root_fsolver_root(s);
        x_lo = gsl_root_fsolver_x_lower(s);
        x_hi = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval(x_lo, x_hi, 0.00001, 0.00001);
        if(status == GSL_SUCCESS)
            printf("ROOTS! BLOODY ROOTS!\n");

        printf("%5d [%11.7f, %11.7f] %11.7f %11.7f %11.7f\n",
        		it, x_lo, x_hi, r, r - r_exp, x_hi - x_lo);
    } while(status == GSL_CONTINUE && it < maxit);

    gsl_root_fsolver_free(s);
}

void roots_fdf(gsl_function_fdf *fun, const gsl_root_fdfsolver_type *T,
		double x, double r_exp, uint maxit) {
	uint it = 0;
	int status;
	gsl_root_fdfsolver *s;
	double x0;

	s = gsl_root_fdfsolver_alloc(T);
	gsl_root_fdfsolver_set(s, fun, x);

	printf("[polishing: %s]\n", gsl_root_fdfsolver_name(s));
	printf("%5s %12s %10s %10s\n",
			"iter", "root", "err", "err(est)");

	do {
		++it;
		status = gsl_root_fdfsolver_iterate(s);
		x0 = x;
		x = gsl_root_fdfsolver_root(s);
		status = gsl_root_test_delta(x, x0, 0, 1e-6);
		if(status == GSL_SUCCESS)
			printf("ROOTS! BLOODY ROOTS!\n");
		printf("%5d %12.7f %+10.7f %10.7f",
				it, x, x - r_exp, x - x0);
		if(status != GSL_SUCCESS && it - 1 < lyrics_s)
			printf("\t%s",lyrics[it-1]);
		printf("\n");
	} while(status == GSL_CONTINUE && it < maxit);
}
