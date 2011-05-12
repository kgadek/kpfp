#include "f_wielomianowa.h"

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
void f_wielomianowa_f_df(double x,void *params,double *f,double *df) {
	*f = f_wielomianowa_def(x,params);		/*nie ma chyba prostej metody na przyspieszenie obliczeń*/
	*df = f_wielomianowa_df(x,params);
}
