#include <stdio.h>
#include <stdlib.h>

int main() {
    int speakout[] = {72, 101, 108, 111, 32, 75, 105, 116, 116, 121, 33};
    unsigned int i;
    printf("C says: ");
    for(i=0; 4*i<sizeof(speakout); i++)
        putchar(speakout[i]);
    putchar('\n');
    return 0;
}

