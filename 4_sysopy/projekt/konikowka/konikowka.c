#include <stdio.h>
#include <pthread.h>
#define w 5
#define h 5
#define WITH_LOCK(lock, x) {pthread_mutex_lock(&lock); {x;} pthread_mutex_unlock(&lock); }

struct prevList {
	struct prevList *prev;
	int pos;
	int no;
};

int x(int);
int y(int);
void *backtrace(void *);

int movex[] = {1, 2, 2, 1, -1, -2, -2, -1};
int movey[] = { -2, -1, 1, 2, 2, 1, -1, -2};
int threads;
int solutions;
pthread_mutex_t printLock;
pthread_mutex_t cntrsLock;

int main() {
	struct prevList head;
	head.pos = 0;
	head.prev = 0;
	head.no = 0;
	solutions = 0;
	threads = 0;
	pthread_mutex_init(&printLock, NULL);
	backtrace(&head);
	printf("Solutions: %d\nThreads: %d\n",solutions, threads);
	return 0;
}

void *backtrace(void *args) {
	struct prevList *prev = (struct prevList*)args;
	struct prevList *tmp;
	struct prevList next[8];
	pthread_t thrdz[8];
	int i, px, py;
	if(prev->no == w*h-1) {
		WITH_LOCK(cntrsLock, ++solutions)
		pthread_mutex_lock(&printLock);
		printf("Sol: ");
		while(prev->prev) {
			printf("%d <-- ", prev->pos);
			prev = prev->prev;
		}
		printf("%d\n", prev->pos);
		pthread_mutex_unlock(&printLock);
	} else {
		px = x(prev->pos);
		py = y(prev->pos);
		for(i=0;i<8;++i) {
			thrdz[i] = 0;
			next[i].no = prev->no + 1;
			next[i].prev = prev;
			if(movex[i]>0) {
				if(px>=w-movex[i]) continue;
			} else {
				if(px<-movex[i]) continue;
			}
			if(movey[i]>0) {
				if(py>=h-movey[i]) continue;
			} else {
				if(py<-movey[i]) continue;
			}
			next[i].pos = prev->pos + movey[i]*w + movex[i];
			for(tmp = prev; tmp != NULL && tmp->pos != next[i].pos; tmp = tmp->prev) ;
			if(tmp == NULL) { /*nie było wcześniej*/
				WITH_LOCK(printLock, { printf("prop%d:%2d -- ",i,next[i].pos); for(tmp=prev;tmp;tmp=tmp->prev) printf("%2d < ",tmp->pos); printf("\n"); })
				pthread_create(&(thrdz[i]), NULL, backtrace, (void*)&next[i]);
				WITH_LOCK(cntrsLock, ++threads)
			}
		}
		for(i=0;i<8;++i) {
			if(thrdz[i] != 0) {
				pthread_join(thrdz[i], NULL);
			}
		}
	}
	return NULL;
}

int x(int p) { return p%w; }
int y(int p) { return p/w; }
