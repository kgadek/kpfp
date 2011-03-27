#include <stdlib.h>
#include <corbusier.h>

/**
 * Le Corbusier - francuski socjalista, architekt, urbanista, rzeźbiarz i
 * malarz. Jest ojcem idei blokowisk (blok mieszkalny).
 *  -- pl.wikipedia.org */


#define BLOCKMEMDIFF 0.2
#define DEFAULTMAXMEM 0

struct lista {
	unsigned int maxMem; /** maksymalna ilość pamięci dopuszczana dla listy *f */
	unsigned int bufMem; /** ilość pamięci zaalokowanej, dostępnej w buforze */
	struct block *x; /** zaalokowane bloku lub NULL */
	struct block *f; /** zwolnione bloki (lista posortowana wg rozmiarów rosnąco o
					  * łącznym rozmiarze nie większym niż maxMem(=0) lub NULL */
};

struct block {
	unsigned int allocated; /** ilość zaalokowanej pamięci */
	struct block *n; /** następny blok lub NULL */
	struct block *p; /** poprzedni blok lub NULL */
	void *x; /** wskazywane miejsce pamięci lub NULL */
};

struct lista* createList() {
	struct lista *t = (struct lista*)malloc(sizeof(struct lista));
	if(!t)
		return t;
	t->maxMem = DEFAULTMAXMEM;
	t->bufMem = 0;
	t->x = 0;
	t->f = 0;
	return t;
}

void deleteList(struct lista **l) {
	struct block *b;
	if(!l || !(*l))
		return;
	while((*l)->x) {
		b = (*l)->x;
		(*l)->x = b->n;
		hardDeleteBlock(&b);
	}
	while((*l)->f) {
		b = (*l)->f;
		(*l)->f = b->n;
		hardDeleteBlock(&b);
	}
	free(*l);
	*l = 0;
}

struct block* newBlock(struct lista *l, unsigned int size) {
	struct block *b, *p;
	double tmp;
	if(!l || !size)
		return 0;
	b = 0;
	for(p=l->f; p && !b; p=p->n) {
		tmp = (double)size/p->allocated;
		if((double)(1-BLOCKMEMDIFF)<=tmp && tmp<=(double)(1+BLOCKMEMDIFF)) {
			b = p;
			/* usuwanie z listy l->f */
			if(b->n)
				b->n->p = b->p;
			if(b->p)
				b->p->n = b->n;
			else
				l->f = b->n;
			l->bufMem -= b->allocated;
		}
	}
	if(!b) {
		b = (struct block*)malloc(sizeof(struct block)); 
		if(!b)
			return 0;
		b->x = 0;
	}
	b->allocated = size;
	b->x = realloc(b->x, size);
	/* dodawanie do listy l->x */
	b->p = 0;
	b->n = l->x;
	if(l->x)
		(l->x)->p = b;
	l->x = b;
	return b;
}

void hardDeleteBlock(struct block **b) {
	if(!b || !(*b))
		return;
	if((*b)->x && (*b)->allocated)
		free((*b)->x);
	(*b)->x = 0;
	(*b)->allocated = 0;
	*b = 0;
}

void checkBufferBlock(struct lista *l) {
	struct block *x, *y;
	unsigned int akt;
	if(!l || l->bufMem <= l->maxMem)
		return;
	/* == heurystyczny algorytm plecakowy == */
	/* w najgorszym przypadku: max_block_size/2 */
	/* czas: O(n) przy założeniu, że lista posortowana */
	akt = 0;
	for(x=l->f; x; x=x->n) {
		if(x->allocated + akt <= l->maxMem)
			akt += x->allocated;
		else {
			if(x->p)
				x->p->n = x->n;
			else
				l->f = x->n;
			if(x->n)
				x->n->p = x->p;
			y = x;
			hardDeleteBlock(&y);
		}
	}
	l->bufMem = akt;
}

void deleteBlock(struct lista *l, struct block **b) {
	struct block *x,*y;
	if(!l || !b || !(*b))
		return;
	if((*b)->p)
		((*b)->p)->n = (*b)->n;
	else {
		if(l->x == (*b))
			l->x = (*b)->n;
		else
			l->f = (*b)->n;
	}
	if((*b)->n)
		((*b)->n)->p = (*b)->p;
	if((*b)->allocated <= l->maxMem) {
		/* wrzuć */
		l->bufMem += (*b)->allocated;
		for(x=0, y=l->f; y && y->allocated < (*b)->allocated; y=y->n)
			x = y;
		(*b)->p = x;
		(*b)->n = y;
		if(y)
			y->p = (*b);
		if(x)
			x->n = (*b);
		else
			l->f = (*b);
		/* czyść */
		checkBufferBlock(l);
		(*b) = 0;
	} else
		hardDeleteBlock(b);
}

int emptyList(struct lista *l) {
	if(!l)
		return -1;
	return !(*l).x;
}

unsigned int allocatedMemorySizeList(struct lista *l) {
	unsigned int s;
	struct block *b;
	if(!l)
		return 0;
	for(s=0, b = l->x; b != 0; b = b->n)
		s += b->allocated;
	return s;
}

unsigned int bufferedMemorySizeList(struct lista *l) {
	if(!l)
		return 0;
	return l->bufMem;
}

unsigned int totalAllocatedMemorySizeList(struct lista *l) {
	return bufferedMemorySizeList(l) + allocatedMemorySizeList(l);
}

void* getValueBlock(struct block *b) {
	return b==0 ? 0 : (void*)b->x;
}

unsigned int getSizeBlock(struct block *b) {
	return b==0 ? 0 : b->allocated;
}

unsigned int getMaxMemList(struct lista *l) {
	return l==0 ? 0 : l->maxMem;
}

unsigned int setMaxMemList(struct lista *l, unsigned int size) {
	unsigned int o = l->maxMem;
	l->maxMem = size;
	checkBufferBlock(l);
	return o;
}

int __selftest_sortedBufList(struct lista *l) {
	unsigned int s;
	struct block *c;
	if(!l || !(l->f))
		return 1;
	c = l->f;
	s = l->f->allocated;
	while(c) {
		if(s > c->allocated) {
			c = 0;
			return 0;
		}
		s = c->allocated;
		c = c->n;
	}
	return 1;
}

