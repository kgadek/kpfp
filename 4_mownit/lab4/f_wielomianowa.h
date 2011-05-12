#ifndef F_WIELOMIANOWA_H_
#define F_WIELOMIANOWA_H_

typedef unsigned int uint;

struct f_wielomianowa_params {				/* ((...(a[n] x + a[n-1]) x + a[n-2]) x + ...) + a[0] */
	uint n;												/*stopień wielomianu*/
	double *a;											/*współczynniki (tablica n+1 liczb)*/
};
double f_wielomianowa_def(double,void*);				/*wartość funkcji*/
double f_wielomianowa_df(double,void*);					/*wartość pochodnej funkcji*/
void f_wielomianowa_f_df(double,void*,double*,double*);	/*obie wartości naraz*/



#endif /* F_WIELOMIANOWA_H_ */
