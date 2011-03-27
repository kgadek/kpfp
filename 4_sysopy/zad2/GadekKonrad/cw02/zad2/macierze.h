#include <corbusier.h>

struct macierzPool;

struct macierz;


struct lista* createMacierzPool();

void deleteMacierzPool(struct macierzPool **);

struct macierz* createMacierz(struct macierzPool *, unsigned int, unsigned int);

void deleteMacierz(struct macierzPool *, struct macierz **);

unsigned int* getRows(struct macierz *);

unsigned int* getCols(struct macierz *);

double* accessMacierz(struct macierz*, unsigned int, unsigned int);

struct macierz* addMacierz(struct macierz*, struct macierz*, struct macierzPool *);

struct macierz* mulMacierz(struct macierz*, struct macierz*, struct macierzPool *);

