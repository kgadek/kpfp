#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>

#define PATHBUFSIZE 1024
#define OPTIONSSIZE 10

enum opcje {
	OPT_V = 1,
	OPT_W = 2
};

int main(int argc, char **argv) {
	int c;
	int opt;
	char *path_to_browse;
	char *extension_to_browse;
	DIR *ddir;
	int file_count;
	int foLks;
	char pathbuf[PATHBUFSIZE];
	struct dirent *cdir;
	struct stat stats;
	int pid;
	int ext_filter_len;
	int dirnamelen;
	char **childenv;
	char childoptions[OPTIONSSIZE];
	int localfiles;

	opt = opterr = ext_filter_len = 0;
	strcpy(childoptions,"-");
	while((c = getopt(argc, argv, "vw")) != -1) {
		switch(c) {
			case 'v':
				if(!(opt & OPT_V))
					strcat(childoptions,"v");
				opt |= OPT_V;
				break;
			case 'w':
				if(!(opt & OPT_W))
					strcat(childoptions,"w");
				opt |= OPT_W;
				break;
			default:
				fprintf(stderr, "Błąd - nierozpoznana opcja %c\n",optopt);
				exit(0);
		}
	}

	extension_to_browse = getenv("EXTENSION_TO_BROWSE");
	if(extension_to_browse)
		ext_filter_len = (int)strlen(extension_to_browse);

	if(!(path_to_browse = getenv("PATH_TO_BROWSE"))) {
		path_to_browse = (char*)malloc(2*sizeof(char));
		strcpy(path_to_browse,".");
	}
	if(!(ddir = opendir(path_to_browse))) {
		fprintf(stderr, "Błąd -- nie można otworzyć katalogu %s\n",path_to_browse);
		/*tutaj czyszczenie env o ile zaalokowane recznie*/
		exit(0);
	}

	childenv = (char**)malloc(2*sizeof(char*));
	childenv[1] = 0;
	childenv[0] = (char*)malloc((PATHBUFSIZE+16)*sizeof(char));

	if(opt & OPT_V)
		printf("O HAI, IM IN UR %s\n",path_to_browse);

	for(file_count = 0, foLks = 0; (cdir = readdir(ddir)); ) {
		if(!strcmp(cdir->d_name, ".") || !strcmp(cdir->d_name, ".."))
			continue;
		snprintf(pathbuf, PATHBUFSIZE, "%s/%s", path_to_browse, cdir->d_name);
		stat(pathbuf, &stats);
		if(S_ISDIR(stats.st_mode)) {
			++foLks;
			if((pid = vfork()) == -1) {
				fprintf(stderr, "Błąd vfork!\n");
				exit(0);
			} else if (!pid) {
				snprintf(childenv[0], PATHBUFSIZE+16, "PATH_TO_BROWSE=%s", pathbuf);
				if(execle(argv[0], argv[0], childoptions, 0, childenv) == -1) {
					fprintf(stderr, "O NOES! I IZ %s %s, I HAS"\
							"PATH_TO_BROWSE=%s EXTENSION_TO_BROWSE=%s\n",
							argv[0], childoptions, path_to_browse, extension_to_browse);
					exit(0);
				}
				exit(0);
			}
		} else {
			if(!extension_to_browse)
				++file_count;
			else {
				dirnamelen = (int)strlen(cdir->d_name);
				if(dirnamelen > ext_filter_len &&
						strstr(cdir->d_name+dirnamelen-ext_filter_len, extension_to_browse))
					++file_count;
			}
		}
	}

	if(opt & OPT_W)
		sleep(15);

	localfiles = file_count;
	while(foLks--) {
		wait(&pid);
		file_count += WEXITSTATUS(pid);
	}
	if(opt & OPT_V)
			printf("KTHX, IN YR %s I CAN HAS %d/%d/%d\n",
					path_to_browse,localfiles,file_count-localfiles, file_count);

	return file_count;
}
