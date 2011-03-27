#include <tests.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <corbusier.h>
#include <tester_common.h>

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
	} else { ++fail; printf("%c[0;31m## FAIL%c[0;0m  | (%4d): %s\n",0x1B,0x1B,__LINE__,#t); }

#define EPS 0.0001
int floatEq(float a, float b) {
	return ((a-b)<EPS || (b-a)<EPS);
}

void make_cor_tests() {
	int ok, fail;
	struct lista *listaA, *listaB;

	struct block *blockA, *blockB, *blockC, *blockD, *blockE, *blockF,
				 *blockG, *blockH, *blockI, *blockJ, *blockK, *blockL,
				 *blockM, *blockN, *blockO, *blockP, *blockQ, *blockR;
	int *tab;
	struct statystyki stA, stB, stC;

	struct lista* (*_createList)();
	void (*_deleteList)(struct lista**);
	struct block* (*_newBlock)(struct lista*, unsigned int);
	void (*_hardDeleteBlock)(struct block**);
	void (*_checkBufferBlock)(struct lista *);
	void (*_deleteBlock)(struct lista *, struct block**);
	int (*_emptyList)(struct lista *);
	unsigned int (*_allocatedMemorySizeList)(struct lista *);
	unsigned int (*_bufferedMemorySizeList)(struct lista *);
	unsigned int (*_totalAllocatedMemorySizeList)(struct lista *);
	void* (*_getValueBlock)(struct block *);
	unsigned int (*_getSizeBlock)(struct block *);
	unsigned int (*_getMaxMemList)(struct lista *);
	unsigned int (*_setMaxMemList)(struct lista *, unsigned int);
	int (*___selftest_sortedBufList)(struct lista *);

#if LIBTYPE == 3
	void* dlHandler = dlopen("libcorbusier.so",RTLD_LAZY);
	*(void**)(&_createList) = dlsym(dlHandler, "createList");
	*(void**)(&_deleteList) = dlsym(dlHandler, "deleteList");
	*(void**)(&_newBlock) = dlsym(dlHandler, "newBlock");
	*(void**)(&_hardDeleteBlock) = dlsym(dlHandler, "hardDeleteBlock");
	*(void**)(&_checkBufferBlock) = dlsym(dlHandler, "checkBufferBlock");
	*(void**)(&_deleteBlock) = dlsym(dlHandler, "deleteBlock");
	*(void**)(&_emptyList) = dlsym(dlHandler, "emptyList");
	*(void**)(&_allocatedMemorySizeList) = dlsym(dlHandler, "allocatedMemorySizeList");
	*(void**)(&_bufferedMemorySizeList) = dlsym(dlHandler, "bufferedMemorySizeList");
	*(void**)(&_totalAllocatedMemorySizeList) = dlsym(dlHandler, "totalAllocatedMemorySizeList");
	*(void**)(&_getValueBlock) = dlsym(dlHandler, "getValueBlock");
	*(void**)(&_getSizeBlock) = dlsym(dlHandler, "getSizeBlock");
	*(void**)(&_getMaxMemList) = dlsym(dlHandler, "getMaxMemList");
	*(void**)(&_setMaxMemList) = dlsym(dlHandler, "setMaxMemList");
	*(void**)(&___selftest_sortedBufList) = dlsym(dlHandler, "__selftest_sortedBufList");
#else
	_createList = createList;
	_deleteList = deleteList;
	_newBlock = newBlock;
	_hardDeleteBlock = hardDeleteBlock;
	_checkBufferBlock = checkBufferBlock;
	_deleteBlock = deleteBlock;
	_emptyList = emptyList;
	_allocatedMemorySizeList = allocatedMemorySizeList;
	_bufferedMemorySizeList = bufferedMemorySizeList;
	_totalAllocatedMemorySizeList = totalAllocatedMemorySizeList;
	_getValueBlock = getValueBlock;
	_getSizeBlock = getSizeBlock;
	_getMaxMemList = getMaxMemList;
	_setMaxMemList = setMaxMemList;
	___selftest_sortedBufList = __selftest_sortedBufList;
#endif

	ok = 0;
	fail = 0;
	tab = 0;
	listaA = 0;
	listaB = 0;

	blockA = 0;
	blockB = 0;
	blockC = 0;
	blockD = 0;
	blockE = 0;
	blockF = 0;
	blockG = 0;
	blockH = 0;
	blockI = 0;
	blockJ = 0;
	blockK = 0;
	blockL = 0;
	blockM = 0;
	blockN = 0;
	blockO = 0;
	blockP = 0;
	blockQ = 0;
	blockR = 0;


	printf("%c[0;1mTesty: start ________________________________________________________[ %-7s ]%c[0;0m\n",0x1B,LIBNAME,0x1B);
	stA = getStats();
	stC = stB = stA;
	printStats(&stA, &stB, &stC);

	TEST(listaA = 0 , _allocatedMemorySizeList(listaA) == 0);
	TEST(; , _emptyList(listaA) == -1);
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 0);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(listaA = _createList(), listaA != 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 0);
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 0);
	TEST(; , _emptyList(listaA) == 1);
	TEST(; , _getSizeBlock(blockA) == 0);
	TEST(; , _getSizeBlock(blockB) == 0);
	TEST(; , _getSizeBlock(blockC) == 0);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(blockA = _newBlock(listaA, sizeof(int)), blockA != 0);
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getSizeBlock(blockB) == 0);
	TEST(; , _getSizeBlock(blockC) == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == sizeof(int));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == sizeof(int));
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getValueBlock(blockA) != 0);
	TEST((*((int*)_getValueBlock(blockA))) = 5, (*((int*)_getValueBlock(blockA))) == 5);
	TEST(; , _emptyList(listaA) == 0);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);


	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(blockB = _newBlock(listaA, sizeof(float)), blockB != 0);
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getSizeBlock(blockB) == sizeof(float));
	TEST(; , _getSizeBlock(blockC) == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getSizeBlock(blockB) == sizeof(float));
	TEST(; , blockB != blockA );
	TEST(; , _getValueBlock(blockB) != 0);
	TEST(; , (*((int*)_getValueBlock(blockA))) == 5);
 	TEST((*((float*)_getValueBlock(blockB))) = (float)7.5, floatEq(*((float*)_getValueBlock(blockB)),7.5));
	TEST(; , _emptyList(listaA) == 0);

	TEST(blockC = _newBlock(listaA, 3*sizeof(int)), blockC != 0);
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getSizeBlock(blockB) == sizeof(float));
	TEST(; , _getSizeBlock(blockC) == 3*sizeof(int));
	TEST(; , _allocatedMemorySizeList(listaA) == 4*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 4*sizeof(int) + sizeof(float));
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getSizeBlock(blockB) == sizeof(float));
	TEST(; , _getSizeBlock(blockC) == 3*sizeof(int));
	TEST(; , blockC != blockA && blockC != blockB);
	TEST(; , _getValueBlock(blockC) != 0);
	TEST(; , (*((int*)_getValueBlock(blockA))) == 5);
 	TEST(; , floatEq(*((float*)_getValueBlock(blockB)), 7.5)); 
	TEST(tab = (int*)_getValueBlock(blockC), tab != 0);
	TEST(tab[0] = 1, tab[0] == 1);
	TEST(tab[1] = 2, tab[1] == 2);
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 2);
	TEST(tab = (int*)_getValueBlock(blockC), tab != 0);
	TEST(tab[0] = 1, tab[0] == 1);
	TEST(tab[1] = 2, tab[1] == 2);
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 2);
	TEST(; , _emptyList(listaA) == 0);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	
	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(_deleteBlock(listaA, &blockC), blockC == 0);
	TEST(; , (*((int*)_getValueBlock(blockA))) == 5);
 	TEST(; , floatEq(*((float*)_getValueBlock(blockB)), 7.5)); 
	TEST(; , _allocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , _getSizeBlock(blockA) == sizeof(int));
	TEST(; , _getSizeBlock(blockB) == sizeof(float));
	TEST(; , _getSizeBlock(blockC) == 0);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);


	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);


	TEST(blockF = _newBlock(listaA, 100*sizeof(int)), blockF != 0);
	TEST(blockD = _newBlock(listaA, 5*sizeof(int)), blockD != 0);
	TEST(blockE = _newBlock(listaA, 10*sizeof(int)), blockE != 0);
	TEST(blockG = _newBlock(listaA, 110*sizeof(int)), blockF != 0);
	TEST(blockH = _newBlock(listaA, 10*sizeof(int)), blockH != 0);
	TEST(blockI = _newBlock(listaA, 2*sizeof(int)), blockI != 0);

	TEST(; , ___selftest_sortedBufList(listaA) == 1);
	TEST(; , _allocatedMemorySizeList(listaA) == 238*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 238*sizeof(int) + sizeof(float));
	TEST(_deleteBlock(listaA, &blockG), blockG == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_setMaxMemList(listaA,20*sizeof(int)+sizeof(float)), _getMaxMemList(listaA) == 20*sizeof(int)+sizeof(float));
	TEST(; , _allocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_deleteBlock(listaA, &blockF), blockF == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_deleteBlock(listaA, &blockE), blockE == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 18*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_deleteBlock(listaA, &blockH), blockH == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 8*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(blockJ = _newBlock(listaA, 2*sizeof(int)), blockJ != 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 10*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 30*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);
	
	TEST(tab = (int*)_getValueBlock(blockJ), tab != 0);
	TEST(tab[0] = 31415, tab[0] == 31415);
	TEST(tab[1] = 27182, tab[1] == 27182);
	TEST(; , tab[0] == 31415);
	TEST(; , tab[1] == 27182);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(blockK = _newBlock(listaA, 9*sizeof(int)), blockK != 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 19*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 29*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);
	TEST(; , tab[0] == 31415);
	TEST(; , tab[1] == 27182);

	TEST(blockL = _newBlock(listaA, 12*sizeof(int)), blockL != 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);
	TEST(; , tab[0] == 31415);
	TEST(; , tab[1] == 27182);

	TEST(blockN = _newBlock(listaA, 10*sizeof(int)), blockN != 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_deleteBlock(listaA, &blockN), blockN == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_deleteBlock(listaA, &blockN), blockN == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	TEST(_setMaxMemList(listaA, 0), _getMaxMemList(listaA) == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , ___selftest_sortedBufList(listaA) == 1);


	TEST(blockP = _newBlock(listaA, sizeof(int)), blockP != 0);
	TEST(blockQ = _newBlock(listaA, sizeof(int)), blockQ != 0);
	TEST(blockR = _newBlock(listaA, sizeof(int)), blockR != 0);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(_deleteList(&listaA), listaA == 0);
	TEST(; , _allocatedMemorySizeList(listaA) == 0);
	TEST(; , _totalAllocatedMemorySizeList(listaA) == 0);
	TEST(; , _emptyList(listaA) == -1);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(blockO = _newBlock(listaA, 2*sizeof(int)), blockO == 0);
	if(blockO)
		_deleteBlock(listaA, &blockO);
	TEST(; , ___selftest_sortedBufList(listaA) == 1);


	blockA = blockB = blockC = blockD = blockE = blockF = blockG = blockH = blockI = blockJ = blockK =\
			 blockL = blockM = blockN = blockO = 0;

	TEST(listaB = 0, 1);
	TEST(; , ___selftest_sortedBufList(listaB) == 1);
	TEST(listaB = _createList(), listaB != 0);
	TEST(_setMaxMemList(listaB, 50*sizeof(int)), _getMaxMemList(listaB) == 50*sizeof(int));
	TEST(;, _emptyList(listaB) == 1);
	TEST(; , ___selftest_sortedBufList(listaB) == 1);
	TEST(blockA = _newBlock(listaB, 2*sizeof(int)), blockA != 0);
	TEST(blockB = _newBlock(listaB, 22*sizeof(int)), blockB != 0);
	TEST(blockC = _newBlock(listaB, 17*sizeof(int)), blockC != 0);
	TEST(blockD = _newBlock(listaB, 35*sizeof(int)), blockD != 0);
	TEST(blockE = _newBlock(listaB, 9*sizeof(int)), blockE != 0);
	TEST(blockF = _newBlock(listaB, 28*sizeof(int)), blockF != 0);
	TEST(blockG = _newBlock(listaB, 10*sizeof(int)), blockG != 0);
	TEST(;, _emptyList(listaB) == 0);
	TEST(blockO = _newBlock(listaB, 0), blockO == 0);
	TEST(;, _emptyList(listaB) == 0);
	TEST(; , ___selftest_sortedBufList(listaB) == 1);

	TEST(; , _allocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(; , _totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(; , ___selftest_sortedBufList(listaB) == 1);

	TEST(tab = (int*)_getValueBlock(blockG) , tab != 0);
	TEST(tab[0] = 1, tab[0] == 1);
	TEST(tab[1] = 1, tab[1] == 1);
	TEST(tab[2] = 2, tab[2] == 2);
	TEST(tab[3] = 3, tab[3] == 3);
	TEST(tab[4] = 5, tab[4] == 5);
	TEST(tab[5] = 8, tab[5] == 8);
	TEST(tab[6] = 13, tab[6] == 13);
	TEST(tab[7] = 21, tab[7] == 21);
	TEST(tab[8] = 34, tab[8] == 34);
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 1);
	TEST(; , tab[2] == 2);
	TEST(; , tab[3] == 3);
	TEST(; , tab[4] == 5);
	TEST(; , tab[5] == 8);
	TEST(; , tab[6] == 13);
	TEST(; , tab[7] == 21);
	TEST(; , tab[8] == 34);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);


	TEST(_deleteBlock(listaB, &blockD), _allocatedMemorySizeList(listaB) == 88*sizeof(int));
	TEST(; , _totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(_deleteBlock(listaB, &blockA), _allocatedMemorySizeList(listaB) == 86*sizeof(int));
	TEST(; , _totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 1);
	TEST(; , tab[2] == 2);
	TEST(; , tab[3] == 3);
	TEST(; , tab[4] == 5);
	TEST(; , tab[5] == 8);
	TEST(; , tab[6] == 13);
	TEST(; , tab[7] == 21);
	TEST(; , tab[8] == 34);
	TEST(_deleteBlock(listaB, &blockG), _allocatedMemorySizeList(listaB) == 76*sizeof(int));
	TEST(; , _totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(_deleteBlock(listaB, &blockB), _allocatedMemorySizeList(listaB) == 54*sizeof(int));
	TEST(; , _totalAllocatedMemorySizeList(listaB) == 88*sizeof(int));
	TEST(; , ___selftest_sortedBufList(listaB) == 1);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

	TEST(_deleteBlock(listaB, &blockC), blockC == 0);
	TEST(_deleteBlock(listaB, &blockE), blockC == 0);
	TEST(_deleteBlock(listaB, &blockF), blockC == 0);
	TEST(_deleteBlock(listaB, &blockO), blockO == 0);
	TEST(;, _emptyList(listaB) == 1);
	TEST(; , ___selftest_sortedBufList(listaB) == 1);

	TEST(_deleteList(&listaB), listaB == 0);
	TEST(; , _allocatedMemorySizeList(listaB) == 0);
	TEST(; , _totalAllocatedMemorySizeList(listaB) == 0);
	TEST(; , _emptyList(listaB) != 0);

	stB = stC;
	stC = getStats();
	printStats(&stA, &stB, &stC);

#if LIBTYPE == 3
	dlclose(dlHandler);
#endif

	printf("%c[0;1mTesty: stop  ________________________________________________________[ %c[0;%dm%-3d/%3d%c[0;1m ]%c[0;0m\n",\
			0x1B, 0x1B, fail?41:42, ok, fail, 0x1B, 0x1B);

}

