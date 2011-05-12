#ifndef F_SIXMIONE_H_
#define F_SIXMIONE_H_

typedef unsigned int uint;

int eqdbl(double,double);
double f_sinxmione_def(double,void*);		/* 1/sin(x) - 1 */
double f_sinxmione_df(double,void*);					/* funkcja nie ma żadnych parametrów */
void f_sinxmione_f_df(double,void*,double*,double*);

#endif /* F_SIXMIONE_H_ */
