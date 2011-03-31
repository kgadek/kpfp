#include <stdio.h>
#include "funkcje.h"

#define LEN 254

int main() {
	char a[LEN] = {'a', 'l', 'a', ' '},
		 b[LEN] = {'m', 'a', ' '},
		 c[LEN] = {'k', 'o', 't', 'a'},
		 d[LEN] = {'a', 'l', 'a', ' '},
		 e[LEN];
	printf("eCompareString: %s\n",eCompareString(a,d) == EQUAL ? "OK" : "FAIL");
	printf("eCompareString: %s\n",eCompareString(a,e) == EQUAL ? "FAIL" : "OK");
	AppendString(b,a);
	AppendString(c,a);
	printf(">[%s]< == >[ala ma kota[< ?\n", a);
	ReplaceCharactersInString(c, 'o', 'a');
	printf(">[%s%s%s]< == >[ala ma kata]< ?\n",d,b,c);
	CopyString(a,e);
	printf("CopyString: %s\n",eCompareString(a,e) == EQUAL ? "OK" : "FAIL");
	return 0;
}

