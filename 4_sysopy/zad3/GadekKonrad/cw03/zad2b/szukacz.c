#include <stdio.h>
#include <unistd.h>
#include <ftw.h>
#include <stdlib.h>
#include <time.h>

#define PATHMAXLEN 2048
#define MAXOPENFD 100
time_t TimeFilter;

int walk(const char *, const struct stat *, int);

int main(int argc, char **argv) {
	char *abspath;
	struct tm tmpdate;

	TimeFilter = 0;
	abspath = 0;

	if(argc == 3)
		TimeFilter = (time_t)atoi(argv[2]);
	else if(argc == 8 || argc == 9) {
		tmpdate.tm_year = atoi(argv[2])-1900;
		tmpdate.tm_mon = atoi(argv[3])-1;
		tmpdate.tm_mday = atoi(argv[4]);
		tmpdate.tm_hour = atoi(argv[5]);
		tmpdate.tm_min = atoi(argv[6]);
		tmpdate.tm_sec = atoi(argv[7]);
		tmpdate.tm_isdst = (argc == 9) ? atoi(argv[8]) == 1 : 1;
		TimeFilter = mktime(&tmpdate);
	} else {
		printf("Wywołanie programu:\n"\
					"\t%s katalog time -- time w sekundach od poczatku Epoki\n"\
					"\t%s katalog yyyy mm dd hh mm ss [dst?1:0]\n"\
				"Wyświetla wszystkie pliki z podanego katalogu o dacie modyfikacji\n"\
				"pozniejszej niz podany czas.\n", argv[0], argv[0]);
		return 1;
	}

	if(chdir(argv[1])) {
		fprintf(stderr, "Nie można przejść do katalogu %s\n",argv[1]);
		return 2;
	}

	if(!(abspath = (char*)malloc(PATHMAXLEN*sizeof(char)))) {
		fprintf(stderr, "Nie można przejść do katalogu %s\n",argv[1]);
		return 3;
	}

	if(!getcwd(abspath,PATHMAXLEN) || ftw(abspath, walk, MAXOPENFD)) {
		free(abspath);
		fprintf(stderr, "Błąd getcwd lub ftw\n");
		return 4;
	}
	free(abspath);

	return 0;
}

int walk(const char *fpath, const struct stat *sb, int typeflag) {
	if(typeflag==FTW_F && sb->st_mtime > TimeFilter)
		printf("%s\n",fpath);
	return 0;
}

