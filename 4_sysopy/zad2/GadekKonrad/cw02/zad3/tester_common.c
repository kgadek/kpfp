#include <tester_common.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <sys/param.h>

#define N 10000

struct statystyki getStats() {
	FILE *f;
	unsigned int i, p,a,b,c;
	char line[512];
	long r;
	struct tms ttt;
	struct statystyki ret;

	f = fopen("/proc/self/stat", "r");				/* pomiar TUTAJ */
	fgets(line,512,f);
	r = times(&ttt);

	fclose(f);
	ret.time = ttt;
	ret.time_real = r;

	for(i=0,p=0; i<22 && line[p] != 0; ++p)
		if(line[p] == ' ')
			++i;
	if(line[p] == 0 || line[++p] == 0) {
		ret.vsize = -1;
		ret.mm = -1;
		return ret;
	}

	a = p;
	for(i=0; i<2 && line[p] != 0; ++p) {
		if(line[p] == ' ') {
			line[p] = 0;
			++i;
			b = c;
			c = p+1;
		}
	}

	ret.vsize = atoi(line+a);
	ret.mm = atoi(line+b);
	return ret;
}

void printStats(struct statystyki *start, struct statystyki *pop, struct statystyki *curr) {
	if(!start || !pop || !curr)
		printf("%70s%c[0;31m-- ERROR%c[0;0m | Błąd pomiaru pamięci / czasu\n"," ",0x1B,0x1B);
	else {
		printf("%70s%c[0;34m-- STATS%c[0;0m | MEMSTAT vsize: %d (%d/%d) mm: %d (%d/%d)\n",\
				" ", 0x1B, 0x1B,\
				curr->vsize, curr->vsize - pop->vsize, curr->vsize - start->vsize,
				curr->mm, curr->mm - pop->mm, curr->mm - start->mm);
		printf("%70s%c[0;34m-- STATS%c[0;0m | CPUSTAT user: %3.2f (%+3.2f/%+3.2f) system: %3.2f (%+3.2f/%+3.2f) real: %3.2f (%+3.2f/%+3.2f)\n",\
				" ", 0x1B, 0x1B,\
				((float)(curr->time.tms_utime))/(N*HZ),
				((float)(curr->time.tms_utime - pop->time.tms_utime))/(N*HZ),
				((float)(curr->time.tms_utime - start->time.tms_utime))/(N*HZ),
				((float)(curr->time.tms_stime))/(N*HZ),
				((float)(curr->time.tms_stime - pop->time.tms_stime))/(N*HZ),
				((float)(curr->time.tms_stime - start->time.tms_stime))/(N*HZ),
				((float)(curr->time_real))/(N*HZ),
				((float)(curr->time_real - pop->time_real))/(N*HZ),
				((float)(curr->time_real - start->time_real))/(N*HZ));
	}
}

