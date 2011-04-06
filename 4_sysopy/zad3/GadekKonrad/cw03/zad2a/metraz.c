#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define PATHMAXLEN 2048

int walk(const char *);

void printHumanitarianSize(int);

int myError(int, char *);

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Wywołanie programu:\n"\
					"\t%s katalog\n"
				"Oblicza sume rozmiarow plikow w podanym katalogu\n", argv[0]);
		return 1;
	}
	printHumanitarianSize(walk(argv[1]));
	return 0;
}

int myError(int errNo, char *buf) {
	fprintf(stderr, "ERR #%03d\n",errNo);
	free(buf);
	return 0;
}

int walk(const char *dirName) {
	DIR *dh;
	struct dirent *dirE;
	struct stat stats;
	int resSize;
	char *pathBuf;
	if(dirName == 0 || !(dh = opendir(dirName)) || !(pathBuf=(char*)malloc(PATHMAXLEN*sizeof(char))))
		return myError(1, 0);
	resSize = 0;
	while((dirE = readdir(dh)) != NULL) {
		if(!strcmp(dirE->d_name,".") || !strcmp(dirE->d_name,".."))
			continue;
		/*printf("walk(%s) dirE=%s\n", dirName, dirE->d_name);*/
		strcpy(pathBuf,dirName); /*pathBuf = dirname + "/" + dirE->d_name*/
		strcat(pathBuf,"/");
		strcat(pathBuf,dirE->d_name);
		if(stat(pathBuf, &stats) == -1)
			continue;
		if(S_ISDIR(stats.st_mode))
			resSize += walk(pathBuf);
		else
			resSize += stats.st_size;
	}
	closedir(dh);
	free(pathBuf);
	return resSize;
}

void printHumanitarianSize(int b) {
	const char pref[] = {' ', 'K', 'M', 'G', 'T', 'P', 'E'};
	int mem[7];
	int i;
	printf("%d B",b);
	if(b>1024) {
		printf("  =");
		for(i=0; b>1024 && i<7; ++i) {
			mem[i] = b%1024;
			b = b/1024;
		}
		mem[i] = b;
		for(; i>0; --i)
			printf("  %d %ciB",mem[i],pref[i]);
		printf("  %d B", mem[0]);
	}
	printf("\n");
}

