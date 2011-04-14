#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>

#define STACKSIZE 4096
int licznik;

int fun(void*);
int doJob(int);

int main(int argc, char **argv) {
	int n;
	int id;
	int args;
	int *stack;
	if(argc != 3 || (n = atoi(argv[1])) < 0 || (id = atoi(argv[2])) < 0 || id > 3) {
		fprintf(stderr, "Błędne wywołanie programu!\n"\
						"Opcje wywołania:\n"\
							"\t%s N <0-3>\n"\
							"\t\tUruchom progam, wykonaj N cykli przy użyciu:\n"\
							"\t\t0 - fork()\n"\
							"\t\t1 - vfork()\n"\
							"\t\t2 - clone() odpowiadający forkowi\n"\
							"\t\t3 - clone() odpowiadający vforkowi\n", argv[0]);
		exit(1);
	}
	licznik = args = 0;
	switch(id) {
		case 0:
			while(n--)
				doJob(fork());
			break;
		case 1:
			while(n--)
				doJob(vfork());
			break;
		case 3:
			args = CLONE_VM | CLONE_VFORK;
		default:
			args |= SIGCHLD;
			stack = (int*)malloc(STACKSIZE*sizeof(int));
			while(n--)
				doJob(clone(fun, stack+STACKSIZE-1, args, 0));
	}
	printf("%d\n", licznik);
	return 0;
}

int fun(void *p) {
	++licznik;
	_exit((int)p);
}

int doJob(int pid) {
    if(pid < 0) {
		fprintf(stderr, "Błąd - nie można wywołać instrukcji fork!\n");
		exit(1);			 /*proces - zonk*/
	} else if(pid != 0)
		return wait(NULL); /*proces - matka*/
	else
		return fun(0); /*proces - dziecko*/
}
