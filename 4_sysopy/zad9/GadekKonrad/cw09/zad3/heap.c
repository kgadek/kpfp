#include "heap.h"
#include <stdlib.h>

#define LEFT(x) (((x)<<1)+1)
#define RIGHT(x) (((x)+1)<<1)
#define PARENT(x) (((x)-1)>>1)

void kpfp_heap_init(struct kpfp_heap *heap, int maxSize) {
	heap->size = 0;
	heap->max = maxSize;
	heap->tab = (int*)malloc((uint)maxSize * sizeof(int));
}

void kpfp_heap_free(struct kpfp_heap *heap) {
	heap->max = 0;
	free(heap->tab);
	head->tab = 0;
}
int kpfp_heap_push(struct kpfp_heap *heap, int val) {
	int i, l, p;
	if(heap->size == heap->max)
		return val-1;
	heap->tab[i=(heap->size)++] = val;
	while(i>0 && heap->tab[p=PARENT(i)] < heap->tab[i]) { /*swap*/
		l = heap->tab[i];
		heap->tab[i] = heap->tab[p];
		heap->tab[i=p] = l;
	}
	return val;
}

int kpfp_heap_popMax(struct kpfp_heap *heap) {
	int res;
	if(!heap->size)
		return 0;
	res = heap->tab[0];
	heap->tab[0] = heap->tab[--(heap->size)];
	_kpfp_heap_heapify(heap, 0);
	return res;
}

int kpfp_heap_getMax(struct kpfp_heap *heap) {
	if(!heap->size)
		return 0;
	return heap->tab[0];
}

void _kpfp_heap_heapify(struct kpfp_heap *heap, int larg) {
	int l, r, i;
	do {
		i = larg;
		l = LEFT(i);
		r = RIGHT(i);
		if(l < heap->size && heap->tab[l] > heap->tab[i])
			larg = l;
		else
			larg = i;
		if(r < heap->size && heap->tab[r] > heap->tab[larg])
			larg = r;
		if(larg != i)
			heap->tab[i] ^= heap->tab[larg] ^= heap->tab[i] ^= heap->tab[larg]; /* można, bo wiemy że mają różne wartości */
	} while(larg != i);
}
