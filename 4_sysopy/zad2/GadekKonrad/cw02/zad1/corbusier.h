#ifndef CORBUSIER_H_
#define CORBUSIER_H_

/**
 * Le Corbusier - francuski socjalista, architekt, urbanista, rzeźbiarz i
 * malarz. Jest ojcem idei blokowisk (blok mieszkalny).
 *  -- pl.wikipedia.org */

struct lista;

struct block;


struct lista* createList();

void deleteList(struct lista **);

struct block* newBlock(struct lista *, unsigned int);

void hardDeleteBlock(struct block **);

void checkBufferBlock(struct lista *);

void deleteBlock(struct lista *, struct block **);

int emptyList(struct lista *);

unsigned int allocatedMemorySizeList(struct lista *);

unsigned int bufferedMemorySizeList(struct lista *);

unsigned int totalAllocatedMemorySizeList(struct lista *);

void* getValueBlock(struct block *);

unsigned int getSizeBlock(struct block *);

unsigned int getMaxMemList(struct lista *);

unsigned int setMaxMemList(struct lista *, unsigned int);

int __selftest_sortedBufList(struct lista *);

#endif

