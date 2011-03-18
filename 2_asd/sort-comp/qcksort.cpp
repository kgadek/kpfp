/** @file qcksort.cpp */
#include "test.h"

/**
 * Algorytm Quick sort, wersja z Wprowadzenia do algorytmów.
 * Czas dzia³ania algorytmu: O(n log n) / Th(n^2)
 * Zu¿ycie pamiêci: O(1)
 * Stabilnoœæ: nie.
 * @author Konrad G¹dek
 * @date 24.03.2010
 */
class QckSort : public Sort {
protected:
	int partition(TestCase& in, int a, int b) {
		int x=in[(a+b)/2];
		//printf("part[%d,%d] by %d = ",a,b,x);
		int i=a-1,j,t;
		for(j=a;j<=b;++j) {
			if(in[j]<=x) {
				t=in[++i];
				in[i]=in[j];
				in[j]=t;
			}
		}
		/*t=in[i];
		in[i]=in[b];
		in[b]=t;*/
		//printf("%d\n",i);
		return i;
	}
	void intsort(TestCase& in, int a, int b) {
		//printf("sort[%d,%d]\n",a,b);
		if(b-a>1) {
			int q=partition(in,a,b);
			try{
				intsort(in,a,q);
				intsort(in,q+1,b);
			}
			catch(...) {}
		}
	}
public:
	void sort(TestCase& in) {
		intsort(in,0,in.size-1);
	}
	void name() {
		printf("Quick Sort");
	}
};