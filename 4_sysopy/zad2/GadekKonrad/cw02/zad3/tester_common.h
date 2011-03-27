#ifndef TESTER_COMMON_H
#define TESTER_COMMON_H

#include <sys/times.h>

struct statystyki {
	struct tms time;
	long time_real;
	int vsize;
	int mm;
};

struct statystyki getStats();

void printStats(struct statystyki *start, struct statystyki *pop, struct statystyki *curr);

#endif

