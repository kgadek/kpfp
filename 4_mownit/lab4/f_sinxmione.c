#include <math.h>
#include <limits.h>
#include "f_sinxmione.h"

#define EPS 0.0001

int eqdbl(double x, double y) {
	return fabs(x-y)<EPS;
}

double f_sinxmione_def(double x,void *params) {
	double s;
	if(!params) {} /*warn-killer*/
	s = sin(x);
	if(eqdbl(s,0.0)) /*s==0?*/
		return 1000000.0;
	return 1.0/s - 1.0;
}
double f_sinxmione_df(double x,void *params) {
	double c, s;
	if(!params) {} /*warn-killer*/
	s = sin(x);
	if(eqdbl(s,0.0)) /*s==0?*/
		return 1000000.0;
	c = cos(x);
	s *= s;
	return -c/s;
}
void f_sinxmione_f_df(double x,void *params,double *f,double *df) {
	double s,c;
	if(!params) {} /*warn-killer*/
	s = sin(x);
	if(eqdbl(s,0.0)) /*s==0?*/
		*f = *df = 1000000.0;
	else {
		c = cos(x);
		*f = 1.0/s - 1.0;
		s *= s;
		*df = -c/s;
	}
}
