/** @file bblsort.cpp */
#include "test.h"

/**
 * Algorytm sortowania b¹belkowego. Przepisany z polskiej wikipedii.
 * Czas dzia³ania algorytmu: Th(n^2).
 * Zu¿ycie pamiêci: O(1)
 * Stabilnoœæ: tak.
 * @author Konrad G¹dek
 * @date 22.03.2010
 */
class WikiBblSort : public Sort {
public:
	void sort(TestCase& in) {
		bool sw;
		int i,s;
		do {
			sw = false;
			for(i=0; i < in.size-1 ; ++i) {
				if(in[i] > in[i+1]) {
					s=in[i];
					in[i]=in[i+1];
					in[i+1]=s;
					sw = true;
				}
			}
		} while (sw);
	}
	void name() {
		printf("Bubble Sort");
	}
};