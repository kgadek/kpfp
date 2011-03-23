#include <stdio.h>
#include <stdlib.h>

#define TABSIZE 1000000

void swap(int *a, int *b) {
    int x;
    x = *a; *a = *b; *b = x;
}

void printTab(int *tab, int len) {
    int i = 0;
    printf("tab: ");
    for(; i<len-1; ++i)
        printf("%d, ", tab[i]);
    printf("%d\n", tab[i]); 
}

void combsort(int*,int);

int main(int argc, char *argv[]) {
    int *t = NULL,i;
	if(argc==1) {
		srand(0);	// generujemy *zawsze* te same liczby
		t = (int*)malloc(TABSIZE * sizeof(int));
		for(i=0;i<TABSIZE;++i)
			t[i] = rand();
    	printTab(t,TABSIZE);
    	combsort(t,TABSIZE);
    	printTab(t,TABSIZE);
	} else {
    	t = (int*)malloc((argc-1) * sizeof(int));
    	for(i=0;i<argc-1;++i)
        	t[i] = atoi(argv[i+1]);
    	printTab(t,argc-1);
    	combsort(t,argc-1);
    	printTab(t,argc-1);
	}
    free(t);
    return 0;
}


void combsort(int *tab, int len) {
    int gap, sw, i;
    gap = len;
    sw = 1;
    while(sw || gap>1) {
        gap = gap*10/13;
        if(!gap)
            gap = 1;
        else if(gap == 9 || gap == 10)
            gap = 11;
        sw = 0;
        for(i = 0; i+gap < len; ++i) {
            if(tab[i+gap]<tab[i]) {
                swap(&tab[i+gap], &tab[i]);
                sw = 1;
            }
        }
    }
}
