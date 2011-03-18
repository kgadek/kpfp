/** @file inssort.cpp */
#include "test.h"

/**
 * Algorytm sortowania przez wstawianie.
 * Czas dzia�ania algorytmu: Om(n) / O(n^2)
 * Zu�ycie pami�ci: O(1)
 * Stabilno��: tak.
 * @author Konrad G�dek
 * @date 24.03.2010
 */
class InsSort : public Sort {
public:
	void sort(TestCase& in) {
		int x,i;
		for(int j=1; j < in.size; ++j) {
			x=in[j];
			for(i=j;i>0 && in[i-1]>x; --i)
				in[i]=in[i-1];
			in[i]=x;
		}
	}
	void name() {
		printf("Insertion Sort");
	}
};