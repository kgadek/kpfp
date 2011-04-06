#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <rygryg.h>

#define CMDLEN 2048
#define ADDCMD(name) else if(!strcmp(cmd, #name)) fn = name;
#define ADDALIAS(name, fun) else if(!strcmp(cmd, #name)) fn = fun;
#define FILENAME 2048

char cmd[CMDLEN];
char filename[CMDLEN];
char filenamebuf[CMDLEN];
int fd;


int main() {
	void (*fn)();

	fd = -1;

	printPS();
	while(fgets(cmd, CMDLEN, stdin)) {
		cmd[strlen(cmd)-1] = 0; /*pozbycie się entera na końcu*/

		if(0) ;
		ADDCMD(setReadLock)
		ADDALIAS(setr, setReadLock)
		ADDALIAS(sr, setReadLock)
		ADDCMD(setWriteLock)
		ADDALIAS(setw, setWriteLock)
		ADDALIAS(sw, setWriteLock)
		ADDCMD(getLocks)
		ADDALIAS(getl, getLocks)
		ADDALIAS(gl, getLocks)
		ADDCMD(releaseLock)
		ADDALIAS(rell, releaseLock)
		ADDALIAS(rl, releaseLock)
		ADDCMD(readFile)
		ADDALIAS(readf, readFile)
		ADDALIAS(rf, readFile)
		ADDALIAS(r, readFile)
		ADDCMD(writeFile)
		ADDALIAS(writef, writeFile)
		ADDALIAS(wf, writeFile)
		ADDALIAS(w, writeFile)
		ADDCMD(quit)
		ADDALIAS(q, quit)
		ADDALIAS(exit, quit)
		ADDALIAS(x, quit)
		ADDCMD(help)
		ADDALIAS(h, help)
		ADDALIAS(?, help)
		else fn = unknownCommand;

		fn();
		printPS();
	}

	return 0;
}


void setWriteLock() {
	int n;
	char numbuf[5];
	getFileName();
	printf("Podaj numer znaku, na który ustawić rygiel: ");
	fgets(numbuf, 5, stdin);
	numbuf[strlen(numbuf)-1] = 0;
	n = atoi(numbuf);
	printf("Ustawiam rygiel zapisu na znak nr %d...", n);
	setLock(F_WRLCK, n, 1, 1);
	printf(" OK\n");
}
void setReadLock() {
	int n;
	char numbuf[5];
	getFileName();
	printf("Podaj numer znaku, na który ustawić rygiel: ");
	fgets(numbuf, 5, stdin);
	numbuf[strlen(numbuf)-1] = 0;
	n = atoi(numbuf);
	printf("Ustawiam rygiel odczytu na znak nr %d...", n);
	setLock(F_RDLCK, n, 1, 1);
	printf(" OK\n");
}
void getLocks() {
	short int type;
	int i, f, whence, start, len, pid, fileLen;
	getFileName();
	fileLen = lseek(fd, 0, SEEK_END);
	f = 0;
	for(i=0; i<fileLen; i+=len) {
		type = F_WRLCK;
		start = i;
		len = 1;
		whence = SEEK_SET;
		getLock(&type, &whence, &start, &len, &pid);
		if(type != F_UNLCK) {
			if(!(f++))
				printf("Rygle na pliku %s (0-%d):\n",filename,fileLen-1);
			printf("\ttype=%s start=%d len=%d pid=%d\n",
					(type==F_RDLCK)?"F_RDLCK":"F_WRLCK",start,len,pid);
		}
	}
	if(!f)
		printf("Brak rygli na pliku %s\n",filename);
}
void releaseLock() {
	int n;
	char numbuf[5];
	getFileName();
	printf("Podaj numer znaku, z którego zdjąć rygiel: ");
	fgets(numbuf, 5, stdin);
	numbuf[strlen(numbuf)-1] = 0;
	n = atoi(numbuf);
	printf("Zdejmuję rygiel ze znaku nr %d...", n);
	n = setLock(F_UNLCK, n, 1, 1);
	if(n == -1)
		printf(" FAIL\n");
	else
		printf(" OK\n");
}
void readFile() {
	char buf[1];
	char numbuf[5];
	int n;
	getFileName();
	printf("Podaj pozycję, z której wczytać znak: ");
	fgets(numbuf, 5, stdin);
	numbuf[strlen(numbuf)-1] = 0;
	n = atoi(numbuf);
	printf("Odczytuję...: ");
	lseek(fd, n, SEEK_SET);
	read(fd,buf,1);
	printf("%c\n",buf[0]);
}
void writeFile() {
	char buf[1];
	char numbuf[5];
	int n;
	getFileName();
	printf("Podaj pozycję, pod którą zapisać znak: ");
	fgets(numbuf, 5, stdin);
	numbuf[strlen(numbuf)-1] = 0;
	n = atoi(numbuf);
	printf("Podaj znak do zapisania: ");
	buf[0] = (char)fgetc(stdin);
	printf("Zapisuję...");
	lseek(fd, n, SEEK_SET);
	write(fd,buf,1);
	printf(" OK\n");
}
void quit() {
	if(fd != -1)
		close(fd);
	exit(0);
}
void unknownCommand() {
	printf("Nierozpoznane polecenie: %s\n", cmd);
	help();
}
void help() {
	printf("Dostępne polecenia:\n"\
				"\tsetWriteLock\n"\
				"\tsetReadLock\n"\
				"\tgetLock\n"\
				"\treleaseLock\n"\
				"\treadFile\n"\
				"\twriteFile\n"\
				"\tquit\n"\
				"\thelp\n");
}

int setLock(short int lockType, int start, int len, short int wait) {
	struct flock lock;
	lock.l_type = lockType;
	lock.l_whence = SEEK_SET;
	lock.l_start = start;
	lock.l_len = len;
	return fcntl(fd, wait?F_SETLKW:F_SETLK, &lock);
}
int getLock(short int *lockType, int *whence, int *start, int *len, int *pid) {
	struct flock lock;
	int ret;
	if(!lockType || !whence || !start || !len || !pid)
		return -1;
	lock.l_type = *lockType;
	lock.l_whence = SEEK_SET;
	lock.l_start = *start;
	lock.l_len = *len;
	lock.l_pid = 0;
	ret = fcntl(fd, F_GETLK, &lock);
	if(ret == -1)
		return ret;
	*lockType = lock.l_type;
	*whence = lock.l_whence;
	*start = lock.l_start;
	*len = lock.l_len;
	*pid = lock.l_pid;
	return ret;
}
void printPS() {
	time_t rawtime;
	char *timeBuf;
	time(&rawtime);
	timeBuf = ctime(&rawtime);
	timeBuf[strlen(timeBuf)-1] = 0;
	printf("%s > ", timeBuf);
}
void getFileName() {
	while(1) {
		if(strlen(filename) != 0)
			printf("Podaj nazwę pliku [%s]: ", filename);
		else
			printf("Podaj nazwę pliku: ");
		fgets(filenamebuf,FILENAME, stdin);
		filenamebuf[strlen(filenamebuf)-1] = 0;
		if(strlen(filenamebuf)) {
			strcpy(filename, filenamebuf);
			if(fd!=-1)
				close(fd);
			fd = open(filename, O_RDWR | O_CREAT);
			if(fd == -1) {
				printf("Nie można otworzyć pliku %s\n", filename);
				continue;
			}
			break;
		} else if(strlen(filename))
			break;
	}
}
