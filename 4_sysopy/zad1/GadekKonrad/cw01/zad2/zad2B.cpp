#include <stdio.h>
#include <stdlib.h>

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
    int *t,i;
    t = (int*)malloc(argc); /* błąd */
    for(i=0;i<argc;++i) /* błąd */
        t[i] = atoi(argv[i]); /* błąd */
    printTab(t,argc); /* błąd */
    combsort(t,argc); /* błąd */
    printTab(t,argc); /* błąd */
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
