#include <tests.h>
#include <stdio.h>
#include <limits.h>
#include <macierze.h>
#include <tester_common.h>
#include <math.h>


#if LIBTYPE == 1
#define LIBNAME "STATIC"

#elif LIBTYPE == 2
#define LIBNAME "SHARED"

#elif LIBTYPE == 3
#include <dlfcn.h>
#define LIBNAME "DYNAMIC"

#else
#define LIBNAME "???"
#endif

#define TEST(op,t) printf("                %-50s    ", #op); op; if(t) {\
		++ok; printf("%c[0;32m--  OK%c[0;0m   | %s\n",0x1B,0x1B,#t);\
	} else { ++fail; printf("%c[0;31m## FAIL%c[0;0m  | (%4d): %s\n",0x1B,0x1B,__LINE__, #t); }

#define EPS 0.0001
int doubleEq(double a, double b) {
	return (double)fabs(a-b)<(double)EPS;
}

void make_mac_tests() {
	int ok, fail;

	struct macierzPool *pA,*pB;
	struct macierz *mA, *mB, *mC, *mD, *mE, *mF, *mG, *mH, *mI, *mJ, *mK;
	struct lista* (*_createMacierzPool)();
	void (*_deleteMacierzPool)(struct macierzPool **);
	struct macierz* (*_createMacierz)(struct macierzPool *, unsigned int, unsigned int);
	void (*_deleteMacierz)(struct macierzPool *, struct macierz**);
	void* (*_accessMacierz)(struct macierz*, unsigned int, unsigned int);
	struct macierz* (*_addMacierz)(struct macierz*, struct macierz*, struct macierzPool *);
	struct macierz* (*_mulMacierz)(struct macierz*, struct macierz*, struct macierzPool *);

	struct statystyki stA, stB, stC;

#if LIBTYPE == 3
	void* dlHandler = dlopen("libmacierze.so", RTLD_LAZY);
	*(void**)(&_createMacierzPool) = dlsym(dlHandler, "createMacierzPool");
	*(void**)(&_createMacierz) = dlsym(dlHandler, "createMacierz");
	*(void**)(&_deleteMacierzPool) = dlsym(dlHandler, "deleteMacierzPool");
	*(void**)(&_deleteMacierz) = dlsym(dlHandler, "deleteMacierz");
	*(void**)(&_accessMacierz) = dlsym(dlHandler, "accessMacierz");
	*(void**)(&_addMacierz) = dlsym(dlHandler, "addMacierz");
	*(void**)(&_mulMacierz) = dlsym(dlHandler, "mulMacierz");
#else
	_createMacierzPool = createMacierzPool;
	_deleteMacierzPool = deleteMacierzPool;
	_createMacierz = createMacierz;
	_deleteMacierz = deleteMacierz;
	_accessMacierz = accessMacierz;
	_addMacierz = addMacierz;
	_mulMacierz = mulMacierz;
#endif

	ok=fail=0;
	pA = pB = 0;
	mA = mB = mC = mD = mE = mF = mG = mH = mI = mJ = mK = 0;

	printf("%c[0;1mTesty: start ________________________________________________________[ %-7s ]%c[0;0m\n",0x1B,LIBNAME,0x1B);
	stA = getStats();
	stC = stB = stA;
	printStats(&stA, &stB, &stC);

	TEST(pA = createMacierzPool(), pA != 0);

	TEST(mA = createMacierz(pA, 2,2), mA != 0);
	TEST(*(double*)_accessMacierz(mA,0,0) = 8, doubleEq(*(double*)_accessMacierz(mA,0,0), 8));
	TEST(*(double*)_accessMacierz(mA,0,1) = 5, doubleEq(*(double*)_accessMacierz(mA,0,1), 5));
	TEST(*(double*)_accessMacierz(mA,1,0) = 13, doubleEq(*(double*)_accessMacierz(mA,1,0), 13));
	TEST(*(double*)_accessMacierz(mA,1,1) = 8, doubleEq(*(double*)_accessMacierz(mA,1,1), 8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,0), 8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,1), 8));

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(mB = _createMacierz(pA, 2,2), mB != 0);
	TEST(*(double*)_accessMacierz(mB,0,0) = -8, doubleEq(*(double*)_accessMacierz(mB,0,0), -8));
	TEST(*(double*)_accessMacierz(mB,0,1) = 5, doubleEq(*(double*)_accessMacierz(mB,0,1), 5));
	TEST(*(double*)_accessMacierz(mB,1,0) = 13, doubleEq(*(double*)_accessMacierz(mB,1,0), 13));
	TEST(*(double*)_accessMacierz(mB,1,1) = -8, doubleEq(*(double*)_accessMacierz(mB,1,1), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,0,0), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,1,1), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,0), 8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,1), 8));

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	mE = mA;
	mF = mB;

	TEST(mC = _mulMacierz(mA, mB, pA), mC != 0);
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,0,0), 1));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,0,1), 0));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,1,0), 0));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,1,1), 1));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,0,0), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,1,1), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,0), 8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,1), 8));
	TEST(; , mA == mE && mB == mF);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(mD = _addMacierz(mA, mB, pA), mD != 0);
	TEST(; , doubleEq(*(double*)_accessMacierz(mD,0,0), 0));
	TEST(; , doubleEq(*(double*)_accessMacierz(mD,0,1), 10));
	TEST(; , doubleEq(*(double*)_accessMacierz(mD,1,0), 26));
	TEST(; , doubleEq(*(double*)_accessMacierz(mD,1,1), 0));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,0,0), 1));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,0,1), 0));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,1,0), 0));
	TEST(; , doubleEq(*(double*)_accessMacierz(mC,1,1), 1));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,0,0), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mB,1,1), -8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,0), 8));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,0,1), 5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,0), 13));
	TEST(; , doubleEq(*(double*)_accessMacierz(mA,1,1), 8));
	TEST(; , mA == mE && mB == mF);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(mG = _createMacierz(pA, 2,3), mG != 0); 
	TEST(*(double*)_accessMacierz(mG,0,0) = 1, doubleEq(*(double*)_accessMacierz(mG,0,0), 1));
	TEST(*(double*)_accessMacierz(mG,0,1) = 0, doubleEq(*(double*)_accessMacierz(mG,0,1), 0));
	TEST(*(double*)_accessMacierz(mG,0,2) = 2, doubleEq(*(double*)_accessMacierz(mG,0,2), 2));
	TEST(*(double*)_accessMacierz(mG,1,0) = -1, doubleEq(*(double*)_accessMacierz(mG,1,0), -1));
	TEST(*(double*)_accessMacierz(mG,1,1) = 3, doubleEq(*(double*)_accessMacierz(mG,1,1), 3));
	TEST(*(double*)_accessMacierz(mG,1,2) = 1, doubleEq(*(double*)_accessMacierz(mG,1,2), 1));
	TEST(mH = _createMacierz(pA, 3,2), mH != 0); 
	TEST(*(double*)_accessMacierz(mH,0,0) = 3, doubleEq(*(double*)_accessMacierz(mH,0,0), 3));
	TEST(*(double*)_accessMacierz(mH,0,1) = 1, doubleEq(*(double*)_accessMacierz(mH,0,1), 1));
	TEST(*(double*)_accessMacierz(mH,1,0) = 2, doubleEq(*(double*)_accessMacierz(mH,1,0), 2));
	TEST(*(double*)_accessMacierz(mH,1,1) = 1, doubleEq(*(double*)_accessMacierz(mH,1,1), 1));
	TEST(*(double*)_accessMacierz(mH,2,0) = 1, doubleEq(*(double*)_accessMacierz(mH,2,0), 1));
	TEST(*(double*)_accessMacierz(mH,2,1) = 0, doubleEq(*(double*)_accessMacierz(mH,2,1), 0));
	TEST(mI = _mulMacierz(mG, mH, pA), mI != 0);
	TEST(; , doubleEq(*(double*)_accessMacierz(mI,0,0),5));
	TEST(; , doubleEq(*(double*)_accessMacierz(mI,0,1),1));
	TEST(; , doubleEq(*(double*)_accessMacierz(mI,1,0),4));
	TEST(; , doubleEq(*(double*)_accessMacierz(mI,1,1),2));

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(mJ = _mulMacierz(mC, mH, pA), mJ == 0);
	TEST(mK = _addMacierz(mG, mA, pA), mK == 0);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(_deleteMacierz(pA, &mA), mA == 0);
	TEST(_deleteMacierz(pA, &mB), mB == 0);
	TEST(_deleteMacierz(pA, &mC), mC == 0);
	TEST(_deleteMacierz(pA, &mD), mD == 0);
	TEST(_deleteMacierz(pA, &mE), mE == 0);
	TEST(_deleteMacierz(pA, &mF), mF == 0);
	TEST(_deleteMacierz(pA, &mG), mG == 0);
	TEST(_deleteMacierz(pA, &mH), mH == 0);
	TEST(_deleteMacierz(pA, &mI), mI == 0);
	TEST(_deleteMacierz(pA, &mJ), mJ == 0);
	TEST(_deleteMacierz(pA, &mK), mK == 0);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(_deleteMacierzPool(&pA), pA == 0);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	printf("%c[0;1mTesty: stop  ________________________________________________________[ %c[0;%dm%-3d/%3d%c[0;1m ]%c[0;0m\n",\
			0x1B, 0x1B, fail?41:42, ok, fail, 0x1B, 0x1B);
#if LIBTYPE == 3
	dlclose(dlHandler);
#endif

}

