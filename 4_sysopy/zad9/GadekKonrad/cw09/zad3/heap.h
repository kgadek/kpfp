#ifndef HEAP_H_
#define HEAP_H_

typedef unsigned int uint;

struct kpfp_heap {
	int size;
	int max;
	int *tab;
};

void kpfp_heap_init(struct kpfp_heap*, int);
int kpfp_heap_push(struct kpfp_heap*, int);
int kpfp_heap_popMax(struct kpfp_heap*);
int kpfp_heap_getMax(struct kpfp_heap*);
void kpfp_heap_free(struct kpfp_heap*);
void _kpfp_heap_heapify(struct kpfp_heap*,int);

#endif /* HEAP_H_ */
