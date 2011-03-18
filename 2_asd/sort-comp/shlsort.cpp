/** @file shlsort.cpp */
#include "test.h"

/**
 * Algorytm Shell sort.
 * Czas dzia³ania algorytmu: O(n log^2 n)
 * Zu¿ycie pamiêci: O(1)
 * Stabilnoœæ: nie.
 * @author Konrad G¹dek
 * @date 24.03.2010
 */
class ShlSort : public Sort {
public:
	void sort(TestCase& in) {
		static const int gaps[] = {1,4,10,23,57,132,301,701,1500,3000,10000}; //11
		int gap,i,x,j;
		for(int k=10;k>=0;--k) {
			gap=gaps[k];
			for(j=gap; j < in.size; ++j) {
				x=in[j];
				for(i=j-gap; i>=0 && in[i]>x; i-=gap)
					in[i+gap]=in[i];
				in[i+gap]=x;
			}
		}
	}
	void name() {
		printf("Shell-Sort");
	}
};