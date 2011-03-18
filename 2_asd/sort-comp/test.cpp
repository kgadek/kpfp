/** @file test.cpp Definicje funkcji testuj¹cych. */

#include "test.h"
#include <ctime>     //clock()
#include <exception> //set_terminate()
#ifdef _WIN32
#include <Windows.h> //QueryPerformanceCounter() - windows only
#endif


ErrOutOfArray::ErrOutOfArray(int _x, int _maxx) : x(_x), maxx(_maxx) {}

/**
 * Terminator wywo³añ wyj¹tków.
 * Zmiana zachowania bloku try..catch - po b³êdzie nie zamyka programu.
 */
void terminator() {}

/**
 * Sprawdza, czy dane posortowano poprawnie.
 * Nie s³u¿y do testowania stabilnoœci algorytmu!
 * @exception ErrBadSortA dane nie zosta³y poprawnie posortowane.
 */
void check(TestCase& in) {
	if(in.size < 1)
		return;
	int i=1;
	int l=in[0];
	while(i < in.size  &&  l <= in[i])
		l = in[i++];
	if(i != in.size)
		throw ErrBadSortA();
}

void Sort::Run(TestCase* in) {
	printf("\t"); name(); printf("... ");
	try {
		in->copy();
#ifdef _WIN32
		LARGE_INTEGER LI_pre, LI_post;
			//http://msdn.microsoft.com/en-us/library/aa383713(VS.85).aspx
		QueryPerformanceCounter(&LI_pre);
			//http://msdn.microsoft.com/en-us/library/ms644904(VS.85).aspx
#endif
		clock_t cl_pre,cl_post;
		cl_pre = clock();
		sort(*in);
#ifdef _WIN32
		QueryPerformanceCounter(&LI_post);
#endif
		cl_post = clock();

		
		check(*in);
#ifdef _WIN32
		printf("(H %lld / L %lld) ",static_cast<long long>(LI_post.HighPart-LI_pre.HighPart),static_cast<long long>(LI_post.LowPart-LI_pre.LowPart));
#endif
		printf("%lld\n",static_cast<unsigned long long>(cl_post)-static_cast<unsigned long long>(cl_pre));
	}
	catch(ErrBadSortA&) {
		printf("\nBLAD! Algorytm zle posortowal dane. Wynik porzadkowania:\n");
		for(int i=0; i < in->size; ++i)
			printf("%d%s",(*in)[i], i < in->size-1?", ":"");
		printf("\n");
	}
	catch(ErrOutOfArray &err) {
		printf("\nBLAD! Proba wyjscia poza tablice. Odwolanie do elementu %d (tablica 0..%d)\n",err.x,err.maxx-1);
	}
	catch(...) {
		printf("\nBLAD! Wynik porzadkowania:\n");
		for(int i=0; i < in->size; ++i)
			printf("%d%s",in[i], i < in->size-1?", ":"");
	}
	in->revert();
}

TestCase::TestCase() {
	id = id_cnt++;
	tab = rev = 0;
	filename = 0;
}
TestCase::TestCase(const char *_fn) {
	id = id_cnt++;
	tab = rev = 0;
	int i=0, s=0;
	while(_fn[s++]) {};
	filename = new char[s];
	for(; i<s; ++i)
		filename[i]=_fn[i];
}
void TestCase::cleanup() {
	if(tab)
		delete [] tab;
	if(rev)
		delete [] rev;
	if(filename)
		delete [] filename;
	tab = rev = 0;
	filename = 0;
}
TestCase::~TestCase() {
	cleanup();
}

int& TestCase::operator [](int x) {
	if(x >= 0 && x < size)
		return tab[x];
	throw ErrOutOfArray(x,size);
}

void TestCase::swap(int *a,int *b) {
	int x=*b;
	*b=*a;
	*a=x;
}

void TestCase_IO::read() {
	if(tab)
		return;
	if(verbose)
		printf("Wprowadz dane: ",id);
	scanf("%d",&size);
	tab = new int[size];
	for(int i=0; i < size; ++i)
		scanf("%d",tab+i);
	printf("\n");
}


TestCase_IO::~TestCase_IO() {
	cleanup();
}

TestCase_ASeries::TestCase_ASeries(int _a, int _b, int _c) : a(_a), b(_b) {
	size = _c;
}

void TestCase_ASeries::copy() {}

void TestCase_ASeries::read() {
	if(!tab)
		tab = new int[size];
	tab[0] = a;
	for(int i=1;i<size;++i)
		tab[i] = tab[i-1] + b;
}

void TestCase_ASeries::revert() {
	if(!tab)
		tab = new int[size];
	tab[0] = a;
	for(int i=1;i<size;++i)
		tab[i] = tab[i-1] + b;
}

TestCase_ASeries::~TestCase_ASeries() {}

void TestCase::copy() {
	if(rev)
		return;
	rev = new int[size];
	for(int i=0;i<size;++i)
		rev[i]=tab[i];
}

void TestCase::read() {
}
void TestCase::revert() {
	if(!rev)
		throw ErrEmptyRev();
		//rev = new int[size];
	for(int i=0;i<size;++i)
		tab[i]=rev[i];
	//delete [] rev;
	//rev = 0;
}

SortComp::~SortComp() {
	TestCase* t;
	while(!tList.empty()) {
		t=tList.front();
		tList.pop_front();
		delete t;
	}
	Sort* a;
	while(!aList.empty()) {
		a=aList.front();
		aList.pop_front();
		delete a;
	}
}

void SortComp::Run() const {
	set_terminate(terminator);
	ListAlg();
	std::list<TestCase*>::iterator t = tList.begin();
	std::list<Sort*>::iterator a;
	for(; t!=tList.end(); ++t) {
		printf("Zestaw danych nr %d\n",(*t)->id);
		(*t)->read();
		(*t)->copy();
		for(a = aList.begin(); a!=aList.end(); ++a) {
			try{
				
				(*a)->Run(*t);
				(*t)->revert();
			}
			catch(ErrEmptyRev&) {
				printf("zonkk");
			}
			catch(...) {
				printf("zonk");
			}
		}
		(*t)->cleanup();
	}
}

void SortComp::ListAlg() const {
	if(!verbose)
		return;
	std::list<Sort*>::iterator it = aList.begin();
	int x = 0;
	printf("Testowane algorytmy: \n\t");
	for(; it!=aList.end(); ++it) {
		if(x)
			printf(", ");	
		++x;
		(*it)->name();
	}
	printf("\n");
}

void SortComp::addAlgorithm(Sort *a) {
	aList.push_back(a);
}

void SortComp::addTest(TestCase *t) {
	tList.push_back(t);
}