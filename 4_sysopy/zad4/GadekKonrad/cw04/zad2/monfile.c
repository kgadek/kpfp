#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define FILENAMEBUFSIZE 2048

int main(int argc, char **argv) {
	unsigned int delay;
	struct stat file_stat;
	int orig_mtime;
	int pid;
	char cmdbuf[2*FILENAMEBUFSIZE+50];
	char copybuf[FILENAMEBUFSIZE];
	if(argc != 3) {
		fprintf(stderr, "Błędne wywołanie programu!\n"\
						"Opcje wywołania:\n"\
							"\t%s plikX delay\n"\
							"\t\tMonitoruje plik (tworząc jego kopię w plikX.tmp) i w razie\n"\
							"\t\tjego modyfikacji przywraca oryginalną zawartość. Okres\n"\
							"\t\tsprawdzania podany jako ostatni argument (w sekundach).\n"\
						"Zakończenie programu: Ctrl-C\n",
						argv[0]);
		exit(1);
	}
	stat(argv[1],&file_stat);
	delay = (unsigned int)atoi(argv[2]);
	snprintf(copybuf, FILENAMEBUFSIZE, "%s.mtime-%d.tmp",argv[1], orig_mtime = file_stat.st_mtime);
	snprintf(cmdbuf, 2*FILENAMEBUFSIZE+50, "cp %s %s", argv[1], copybuf);
	if(system(cmdbuf) == -1)
		exit(1);
	do {
		stat(argv[1],&file_stat);
		if(file_stat.st_mtime != orig_mtime) {
			if(!(pid = vfork())) {
				if(execlp("/bin/cp","cp",copybuf, argv[1], 0) == -1) {
					fprintf(stderr, "Błąd przywracania!\n");
					exit(1);
				} else if(pid < 0) {
					fprintf(stderr, "Błąd przywracania!\n");
					exit(2);
				}
				wait(0);
				exit(0);
			}
		}
		stat(argv[1], &file_stat);
		orig_mtime = file_stat.st_mtime;
	} while(!sleep(delay));
	return 0;
}
