/** @file sortcomp.cpp Plik programu testuj¹cego. */
#include <cstdio>
#include <list>
#include "test.h"
#include "cmbsort.cpp"
#include "bblsort.cpp"
#include "inssort.cpp"
#include "selsort.cpp"
#include "shlsort.cpp"
#include "mrgsort.cpp"
#include "qcksort.cpp"

int TestCase::id_cnt = 1;
std::list<TestCase*> SortComp::tList;
std::list<Sort*> SortComp::aList;

int verbose = 1; /**< Znacznik trybu verbose. 0 - wy³¹czony. Dowolny inny - w³¹czony. */
SortComp Tester; /**< Reprezentant klasy SortComp. */

/**
 * G³ówny program.
 */
int main() {
	/* ___________ [ Lista zestawów danych ] ___________ */
	Tester.addTest(new TestCase_IO);
	Tester.addTest(new TestCase_ASeries(100,-1,10000));
	Tester.addTest(new TestCase_ASeries(0,1,10000));
	/* ___________ [ Lista algorytmów ] ________________ */
	Tester.addAlgorithm(new WikiCmbSort11);
	//Tester.addAlgorithm(new WikiBblSort);
	Tester.addAlgorithm(new InsSort);
	Tester.addAlgorithm(new SelSort);
	Tester.addAlgorithm(new ShlSort);
	//Tester.addAlgorithm(new MrgSort);
	Tester.addAlgorithm(new QckSort);
	/* ___________ [ Rozpoczêcie testów ] ______________ */
	Tester.Run();
	return 0;
}
