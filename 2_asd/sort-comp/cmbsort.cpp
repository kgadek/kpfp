/** @file cmbsort.cpp  */
#include "test.h"

/**
 * Algorytm sortowania grzebieniowego. Przepisany z polskiej wikipedii.
 * Czas dzia³ania algorytmu: prawdopodobnie O(n log n).
 * Zu¿ycie pamiêci: O(1)
 * Stabilnoœæ: nie.
 * @author Konrad G¹dek
 * @date 19.03.2010
 */
class WikiCmbSort11 : public Sort {
public:
	void sort(TestCase& in) {
		int gap = in.size, tmp;
		bool sw = true;
		while(gap>1 || sw) {
			gap = gap*10/13;
			if(gap==9||gap==10)
				gap = 11;
			else if(!gap)
				gap = 1;
			sw = false;
			for(int i = 0;i+gap<in.size;++i) {
				if(in[i+gap] < in[i]) {
					tmp = in[i];
					in[i] = in[i+gap];
					in[i+gap] = tmp;
					sw = true;
				}
			}
		}
	}
	void name() {
		printf("Comb Sort 11");
	}
};