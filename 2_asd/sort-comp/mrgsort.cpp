/** @file mrgsort.cpp */
#include "test.h"

/**
 * Algorytm Shell sort.
 * Czas dzia³ania algorytmu: O(n log^2 n)
 * Zu¿ycie pamiêci: O(1)
 * Stabilnoœæ: nie.
 * @author Konrad G¹dek
 * @date 24.03.2010
 * @bug nie dzia³a...
 */
class MrgSort : public Sort {
protected:
	void intsort(int a,int b, TestCase& in) {
		if(a<b) {
			int q = (a+b)/2;
			intsort(a,q,in);
			intsort(q+1,b,in);
			int *t = new int[b-a+1];
			int i=0,j=q,c=0;
			for(;c<b-a+1;++c) {
				if((i<=q && in[i]<=in[j]) || j>b)
					t[a+c]=in[i++];
				else
					t[a+c]=in[j++];
			}
			for(i=0;i<b-a+1;++i)
				in[a+i]=t[i];
			delete [] t;
		}
	}
public:
	void sort(TestCase& in) {
		if(in.size>0) {
			int q = in.size/2;
			intsort(0,q,in);
			intsort(q+1,in.size,in);
			//
		}
	}
	void name() {
		printf("Merge Sort");
	}
};