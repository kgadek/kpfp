#include <stdlib.h>
#include <macierze.h>
#include <corbusier.h>

struct macierzPool {
	struct lista *l;
};

struct macierz {
	struct block *b;
};

struct lista* createMacierzPool() {
	struct macierzPool *m;
	if(!(m = (struct macierzPool*)malloc(sizeof(struct macierzPool))))
		return 0;
	m->l = createList();
	if(!(m->l)) {
		free(m);
		return 0;
	}
	return m;
}

void deleteMacierzPool(struct macierzPool **pl) {
	if(!pl || !(*pl) || !((*pl)->l))
		return;
	deleteList(&((*pl)->l));
	(*pl) = 0;
}

struct macierz* createMacierz(struct macierzPool *p, unsigned int w, unsigned int k) {
	struct macierz *m;
	if(!p || !(p->l) || !w || !k || !(m = (struct macierz*)malloc(sizeof(struct macierz))))
		return 0;
	m->b = newBlock(p->l, 2*sizeof(unsigned int)+w*k*sizeof(double));
	if(!(m->b)) {
		free(m);
		return 0;
	}
	*getRows(m) = w;
	*getCols(m) = k;
	return m;
}

void deleteMacierz(struct macierzPool *P, struct macierz **A) {
	if(!P || !A || !(*A))
		return;
	deleteBlock(P->l, &((*A)->b));
	(*A) = 0;
}

unsigned int* getRows(struct macierz *A) {
	unsigned int *tab;
	if(!A || !(A->b))
		return 0;
	tab = (unsigned int*)getValueBlock(A->b);
	return tab;
}

unsigned int* getCols(struct macierz *A) {
	unsigned int *tab;
	if(!A || !(A->b))
		return 0;
	tab = (unsigned int*)getValueBlock(A->b);
	return tab+1;
}

double* accessMacierz(struct macierz *A, unsigned int w, unsigned int k) {
	unsigned int cols;
	double *tab;
	if(!A || *getRows(A) <= w || (cols = *getCols(A)) <= k)
		return 0;
	tab = (double*)((unsigned int*)getValueBlock(A->b)+2);
	return tab+w*cols+k;
}

struct macierz* addMacierz(struct macierz *A, struct macierz *B, struct macierzPool *p) {
	unsigned int i, j,cols,rows,cur;
	struct macierz *r;
	double *tabA, *tabB, *tabC;
	if(!A || !B || !p || (cols=*getCols(A))!=*getCols(B) || (rows=*getRows(A))!=*getRows(B) || !(r = createMacierz(p,rows,cols)))
		return 0;
	tabA = (double*)((unsigned int*)getValueBlock(A->b)+2);
	tabB = (double*)((unsigned int*)getValueBlock(B->b)+2);
	tabC = (double*)((unsigned int*)getValueBlock(r->b)+2);
	for(cur=0,i=0;i<rows;i++)
		for(j=0;j<cols;j++,cur++)
			tabC[cur] = tabA[cur]+tabB[cur];
	return r;
}

struct macierz* mulMacierz(struct macierz *A, struct macierz *B, struct macierzPool *p) {
	unsigned int i, j, k, cols,colsB,rows,cur;
	struct macierz *r;
	double *tabA, *tabB, *tabC, tmp;
	if(!A || !B || !p || (cols=*getCols(A))!=(rows=*getRows(B)) || !(r = createMacierz(p,rows,cols)))
		return 0;
	colsB = *getCols(B);
	tabA = (double*)((unsigned int*)getValueBlock(A->b)+2);
	tabB = (double*)((unsigned int*)getValueBlock(B->b)+2);
	tabC = (double*)((unsigned int*)getValueBlock(r->b)+2);
	for(cur=0,i=0;i<rows;i++)
		for(j=0;j<cols;j++,cur++) {
			for(k=0,tmp=0;k<cols;k++)
				tmp += tabA[i * cols+k] * tabB[k * colsB+j];
			tabC[cur] = tmp;
		}
	return r;
}

