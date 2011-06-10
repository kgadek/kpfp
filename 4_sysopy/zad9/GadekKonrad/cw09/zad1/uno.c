#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

#define myerror(str,myerrno) {\
                        fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
                        exit(myerrno);\
                }
#define myerrorpth(str,myerrno) {\
                        fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
                        pthread_exit((void*)myerrno);\
                }
#define MYNEW(n,type) ((type*)malloc((uint)n*sizeof(type)))
typedef unsigned int uint;


int fd;
int thrdzCnt;
int bufSize;
int dl;
char *str;
pthread_t* thrdz;
pthread_attr_t thrdzAttrs;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

struct thrdInfo{
	char* buf;
	int idx;
};


void* searchForThatNeedle(void* args);
void thrdzCleanup(void *bufor);
void myatexit(void);


int main(int argc, char* argv[]){
	int i;
	int tmp;
	
	if(argc != 5)
		myerror("Błędna liczba argumentów (podaj ilość wątków, nazwę pliku, rozmiar buforów i wzorzec)",1);
	thrdzCnt = atoi(argv[1]);
	if(thrdzCnt < 1)
		myerror("Niedodatnia ilość wątków!",2);
	tmp = sysconf(_SC_THREAD_THREADS_MAX);
	if(tmp != -1 && thrdzCnt > tmp) {
		fprintf(stderr, "Maksymalna ilość wątków (sysconf(_SC_THREAD_THREADS_MAX)) : %d\n",tmp);
		myerror("Podano zbyt dużą ilość wątków!",3);
	}	
	fd = open(argv[2], O_RDONLY);
	if(fd == -1)
		myerror("Błąd otwierania pliku!",4);
	bufSize = atoi(argv[3]);
	if(bufSize < 1)
		myerror("Niedodatni rozmiar bufora!",5);
	str = argv[4];
	atexit(myatexit);
	thrdz = MYNEW(thrdzCnt, pthread_t);
	tmp = pthread_attr_init(&thrdzAttrs);
	if(tmp != 0)
		myerror("Błąd pthread_attr_init(3p)!",6);

	
#ifdef wersjaTrzecia
	tmp = pthread_attr_setdetachstate(&thrdzAttrs, PTHREAD_CREATE_DETACHED);
	if(tmp)
		myerror("Błąd pthread_attr_setdetachstate(3p)!",6);
#endif
	
	for(i=0; i<thrdzCnt; ++i){
		tmp = pthread_create (&thrdz[i], &thrdzAttrs, &searchForThatNeedle, (void*)i);
		if(tmp == -1)
			myerror("Błąd pthread_create(3p)!",7);
	}
	
	sleep(3); /*dajmy im chwilę na rozgrzewkę, w końcu wątki też ludzie*/
	
#ifndef wersjaTrzecia	
	for(i=0; i<thrdzCnt; ++i) {
		if((thrdz[i] != 0) && pthread_join(thrdz[i], NULL) != 0)
			myerror("Błąd pthread_join(3p)!",8);
	}
#endif
	pthread_attr_destroy(&thrdzAttrs);
  	free(thrdz);
	return 0;
}


void thrdzCleanup(void *dane){
	free(((struct thrdInfo*)dane)->buf); /*Boeing 777 łanlajner*/
}


void myatexit(){
	if(close(fd) == -1)
		fprintf(stderr,"Błąd close(2)!\n");
	free(thrdz);
	if(pthread_attr_destroy(&thrdzAttrs) != 0)
		fprintf(stderr,"Błąd pthread_attr_destroy(3p)!\n");
}


void* searchForThatNeedle(void* args){
	int i;
	int j;
	int k;
	int tmp;
	struct thrdInfo myThreadyData;
	int readLen;
	int readBegin;
	int readEnd;
	int ok;
	int cntr;

#ifdef wersjaPierwsza
	tmp = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); /*asynchroniczne anulowanie wątków*/
#elif wersjaDruga
	tmp = thread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); /*synchroniczne anulowanie wątków (domyślne ale niech już będzie napisane explicite)*/
#else /*domyślnie wersjaTrzecia*/
	tmp = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); /*wszystkie wątkami odłączone (niegrzecznie ignorują zadania odwołania do czasu aż będzie im się chciało)*/
#endif
	if(tmp != 0)
		myerrorpth("Błąd pthread_setcanceltype(3p)",9);

	myThreadyData.idx = (int)args;
	myThreadyData.buf = MYNEW(bufSize, char);
	pthread_cleanup_push(thrdzCleanup, &myThreadyData);

	for(;;) {
		pthread_mutex_lock(&mutex); /*ympffff...*/
		readBegin = lseek(fd, 0, SEEK_CUR);
		readLen = read(fd,myThreadyData.buf,sizeof(char)*(uint)bufSize);
		pthread_mutex_unlock(&mutex); /*...ffff! (atomowy okrzyk pana Mietka spod alkoholowego)*/
		if(readLen == -1) {
			myerrorpth("Błąd read(2)!",10);
		} else if(!readLen)
			return NULL;
	
		for(i=0; i<readLen; i++) { /*Panowie Knuth, Morris i Pratt raczej nie polubią tego algorytmu...*/
			for(j=i; ((uint)(j-i)<strlen(str)) && (j<readLen); ++j)
				if(myThreadyData.buf[j] != str[j-i])
					break;
			ok = (uint)(j-i) >= strlen(str);
	
			if(((uint)(j-i) == strlen(str))) {
				printf("I IZ %d, CAN HAZ STRZ HERZ: %d\n", (int)pthread_self(), readBegin + i);
#ifndef wersjaTrzecia	
				if(pthread_mutex_trylock(&mutex2) != EBUSY){
					for(k=0; k<thrdzCnt; k++){
						if((k != myThreadyData.idx)&& (thrdz[k] != 0)){
							pthread_cancel(thrdz[k]);
							thrdz[k] = 0;
						}
					}
				}
				thrdz[myThreadyData.idx] = 0;
				pthread_exit((void*)0);
#endif
			}
		}
		
#ifdef wersjaDruga
		pthread_testcancel();
#endif
		
		for(i=readLen-(int)strlen(str); i<readLen; ++i){
			ok = 1;
			for(j=i; j<readLen; ++j)
				if(myThreadyData.buf[j] != str[j-i])
					break;
			dl = j-i;
			if(j>=readLen) {
				cntr = (int)strlen(str)+j-i;
			
				pthread_mutex_lock(&mutex); /*ympffff...*/
				readEnd = lseek(fd,0,SEEK_CUR);
				lseek(fd, readBegin + readLen,SEEK_SET);
				readLen = read(fd, myThreadyData.buf, sizeof(char)*(uint)bufSize);
				lseek(fd, readEnd, SEEK_SET);
				pthread_mutex_unlock(&mutex); /*...ffff! (atomowy wrzask udomowionego legwana oglądającego horror na TVN w poniedziałek)*/
				
				if (readLen == -1)
					pthread_exit((void*)1);
				else if(!readLen) {
					thrdz[myThreadyData.idx] = 0;
					pthread_exit((void*)0);
				}
				
				for (j=0; (j<cntr) && (j<readLen) && ((uint)dl<strlen(str)); ++j){
                    if(myThreadyData.buf[j]!= str[dl++]){
                		ok=0; 
               			break;
                    }
				}
                
                if(ok && ((uint)dl == strlen(str))){
                	printf("I IZ %d, CAN HAZ STRZ HERZ: %d\n", (int)pthread_self(), readBegin + i);
#ifndef wersjaTrzecia	
					if(pthread_mutex_trylock(&mutex2) != EBUSY) {
						for(k=0; k<thrdzCnt; k++){
							if((k != myThreadyData.idx) && thrdz[k]){
								pthread_cancel(thrdz[k]);
								thrdz[k] = 0;
							}
						}
					}
					thrdz[myThreadyData.idx] = 0;
					pthread_exit((void*)0);
#endif
                } /*if(ok && ((uint)dl == strlen(str)))*/
			} /*if(j>=readLen)*/
#ifdef wersjaDruga
		pthread_testcancel();
#endif
		} /*for(i=readLen-(int)strlen(str); i<readLen; ++i)*/
	} /*for(;;)*/
	pthread_cleanup_pop(1); /*w/handler function*/
	return NULL;
}

