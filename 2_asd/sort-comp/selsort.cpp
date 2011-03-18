/** @file selsort.cpp */
#include "test.h"

/**
 * Algorytm sortowania przez wyb�r.
 * Czas dzia�ania algorytmu: O(n^2)
 * Zu�ycie pami�ci: O(1)
 * Stabilno��: tak.
 * @author Konrad G�dek
 * @date 24.03.2010
 */
class SelSort : public Sort {
public:
	void sort(TestCase& in) {
		int mx,j;
		for(int i=0;i<in.size-1;++i) {
			mx=i;
			for(j=i+1;j<in.size;++j) {
				if(in[j]<in[mx])
					mx=j;
			}
			j=in[i];
			in[i]=in[mx];
			in[mx]=j;
		}
	}
	void name() {
		printf("Selection Sort");
	}
};