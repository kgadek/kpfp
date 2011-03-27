#include <tests.h>
#include <stdio.h>
#include <limits.h>
#include <corbusier.h>

#if LIBTYPE == 1
#define LIBNAME "STATIC"
#elif LIBTYPE == 2
#define LIBNAME "SHARED"
#else
#define LIBNAME ""
#endif

#define TEST(op,t) printf("                %-50s    ", #op); op; if(t) {\
		++ok; printf("%c[0;32m--  OK%c[0;0m   | %s\n",0x1B,0x1B,#t);\
	} else { ++fail; printf("%c[0;31m## FAIL%c[0;0m  | %s\n",0x1B,0x1B,#t); }

#define EPS 0.0001
int floatEq(float a, float b) {
	return ((a-b)<EPS || (b-a)<EPS);
}

void make_tests() {
	int ok, fail;
	struct lista *listaA, *listaB;

	struct block *blockA, *blockB, *blockC, *blockD, *blockE, *blockF,
				 *blockG, *blockH, *blockI, *blockJ, *blockK, *blockL,
				 *blockM, *blockN, *blockO, *blockP, *blockQ, *blockR;
	int *tab;

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


	TEST(listaA = 0 , allocatedMemorySizeList(listaA) == 0);
	TEST(; , emptyList(listaA) == -1);
	TEST(; , totalAllocatedMemorySizeList(listaA) == 0);
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(listaA = createList(), listaA != 0);
	TEST(; , allocatedMemorySizeList(listaA) == 0);
	TEST(; , totalAllocatedMemorySizeList(listaA) == 0);
	TEST(; , emptyList(listaA) == 1);
	TEST(; , getSizeBlock(blockA) == 0);
	TEST(; , getSizeBlock(blockB) == 0);
	TEST(; , getSizeBlock(blockC) == 0);
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(blockA = newBlock(listaA, sizeof(int)), blockA != 0);
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getSizeBlock(blockB) == 0);
	TEST(; , getSizeBlock(blockC) == 0);
	TEST(; , allocatedMemorySizeList(listaA) == sizeof(int));
	TEST(; , totalAllocatedMemorySizeList(listaA) == sizeof(int));
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getValueBlock(blockA) != 0);
	TEST((*((int*)getValueBlock(blockA))) = 5, (*((int*)getValueBlock(blockA))) == 5);
	TEST(; , emptyList(listaA) == 0);
	TEST(; , __selftest_sortedBufList(listaA) == 1);


	TEST(blockB = newBlock(listaA, sizeof(float)), blockB != 0);
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getSizeBlock(blockB) == sizeof(float));
	TEST(; , getSizeBlock(blockC) == 0);
	TEST(; , allocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getSizeBlock(blockB) == sizeof(float));
	TEST(; , blockB != blockA );
	TEST(; , getValueBlock(blockB) != 0);
	TEST(; , (*((int*)getValueBlock(blockA))) == 5);
 	TEST((*((float*)getValueBlock(blockB))) = (float)7.5, floatEq(*((float*)getValueBlock(blockB)),7.5));
	TEST(; , emptyList(listaA) == 0);

	TEST(blockC = newBlock(listaA, 3*sizeof(int)), blockC != 0);
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getSizeBlock(blockB) == sizeof(float));
	TEST(; , getSizeBlock(blockC) == 3*sizeof(int));
	TEST(; , allocatedMemorySizeList(listaA) == 4*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 4*sizeof(int) + sizeof(float));
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getSizeBlock(blockB) == sizeof(float));
	TEST(; , getSizeBlock(blockC) == 3*sizeof(int));
	TEST(; , blockC != blockA && blockC != blockB);
	TEST(; , getValueBlock(blockC) != 0);
	TEST(; , (*((int*)getValueBlock(blockA))) == 5);
 	TEST(; , floatEq(*((float*)getValueBlock(blockB)), 7.5)); 
	TEST(tab = (int*)getValueBlock(blockC), tab != 0);
	TEST(tab[0] = 1, tab[0] == 1);
	TEST(tab[1] = 2, tab[1] == 2);
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 2);
	TEST(tab = (int*)getValueBlock(blockC), tab != 0);
	TEST(tab[0] = 1, tab[0] == 1);
	TEST(tab[1] = 2, tab[1] == 2);
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 2);
	TEST(; , emptyList(listaA) == 0);
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	
	TEST(deleteBlock(listaA, &blockC), blockC == 0);
	TEST(; , (*((int*)getValueBlock(blockA))) == 5);
 	TEST(; , floatEq(*((float*)getValueBlock(blockB)), 7.5)); 
	TEST(; , allocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == sizeof(int) + sizeof(float));
	TEST(; , getSizeBlock(blockA) == sizeof(int));
	TEST(; , getSizeBlock(blockB) == sizeof(float));
	TEST(; , getSizeBlock(blockC) == 0);
	TEST(; , __selftest_sortedBufList(listaA) == 1);



	TEST(blockF = newBlock(listaA, 100*sizeof(int)), blockF != 0);
	TEST(blockD = newBlock(listaA, 5*sizeof(int)), blockD != 0);
	TEST(blockE = newBlock(listaA, 10*sizeof(int)), blockE != 0);
	TEST(blockG = newBlock(listaA, 110*sizeof(int)), blockF != 0);
	TEST(blockH = newBlock(listaA, 10*sizeof(int)), blockH != 0);
	TEST(blockI = newBlock(listaA, 2*sizeof(int)), blockI != 0);

	TEST(; , __selftest_sortedBufList(listaA) == 1);
	TEST(; , allocatedMemorySizeList(listaA) == 238*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 238*sizeof(int) + sizeof(float));
	TEST(deleteBlock(listaA, &blockG), blockG == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(setMaxMemList(listaA,20*sizeof(int)+sizeof(float)), getMaxMemList(listaA) == 20*sizeof(int)+sizeof(float));
	TEST(; , allocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 128*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(deleteBlock(listaA, &blockF), blockF == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(deleteBlock(listaA, &blockE), blockE == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 18*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(deleteBlock(listaA, &blockH), blockH == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 8*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 28*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(blockJ = newBlock(listaA, 2*sizeof(int)), blockJ != 0);
	TEST(; , allocatedMemorySizeList(listaA) == 10*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 30*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);
	
	TEST(tab = (int*)getValueBlock(blockJ), tab != 0);
	TEST(tab[0] = 31415, tab[0] == 31415);
	TEST(tab[1] = 27182, tab[1] == 27182);
	TEST(; , tab[0] == 31415);
	TEST(; , tab[1] == 27182);

	TEST(blockK = newBlock(listaA, 9*sizeof(int)), blockK != 0);
	TEST(; , allocatedMemorySizeList(listaA) == 19*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 29*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);
	TEST(; , tab[0] == 31415);
	TEST(; , tab[1] == 27182);

	TEST(blockL = newBlock(listaA, 12*sizeof(int)), blockL != 0);
	TEST(; , allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);
	TEST(; , tab[0] == 31415);
	TEST(; , tab[1] == 27182);

	TEST(blockN = newBlock(listaA, 10*sizeof(int)), blockN != 0);
	TEST(; , allocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(deleteBlock(listaA, &blockN), blockN == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(deleteBlock(listaA, &blockN), blockN == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 41*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(setMaxMemList(listaA, 0), getMaxMemList(listaA) == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , totalAllocatedMemorySizeList(listaA) == 31*sizeof(int) + sizeof(float));
	TEST(; , __selftest_sortedBufList(listaA) == 1);


	TEST(blockP = newBlock(listaA, sizeof(int)), blockP != 0);
	TEST(blockQ = newBlock(listaA, sizeof(int)), blockQ != 0);
	TEST(blockR = newBlock(listaA, sizeof(int)), blockR != 0);
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(deleteList(&listaA), listaA == 0);
	TEST(; , allocatedMemorySizeList(listaA) == 0);
	TEST(; , totalAllocatedMemorySizeList(listaA) == 0);
	TEST(; , emptyList(listaA) == -1);
	TEST(; , __selftest_sortedBufList(listaA) == 1);

	TEST(blockO = newBlock(listaA, 2*sizeof(int)), blockO == 0);
	if(blockO)
		deleteBlock(listaA, &blockO);
	TEST(; , __selftest_sortedBufList(listaA) == 1);




	blockA = blockB = blockC = blockD = blockE = blockF = blockG = blockH = blockI = blockJ = blockK =\
			 blockL = blockM = blockN = blockO = 0;

	TEST(listaB = 0, 1);
	TEST(; , __selftest_sortedBufList(listaB) == 1);
	TEST(listaB = createList(), listaB != 0);
	TEST(setMaxMemList(listaB, 50*sizeof(int)), getMaxMemList(listaB) == 50*sizeof(int));
	TEST(;, emptyList(listaB) == 1);
	TEST(; , __selftest_sortedBufList(listaB) == 1);
	TEST(blockA = newBlock(listaB, 2*sizeof(int)), blockA != 0);
	TEST(blockB = newBlock(listaB, 22*sizeof(int)), blockB != 0);
	TEST(blockC = newBlock(listaB, 17*sizeof(int)), blockC != 0);
	TEST(blockD = newBlock(listaB, 35*sizeof(int)), blockD != 0);
	TEST(blockE = newBlock(listaB, 9*sizeof(int)), blockE != 0);
	TEST(blockF = newBlock(listaB, 28*sizeof(int)), blockF != 0);
	TEST(blockG = newBlock(listaB, 10*sizeof(int)), blockG != 0);
	TEST(;, emptyList(listaB) == 0);
	TEST(blockO = newBlock(listaB, 0), blockO == 0);
	TEST(;, emptyList(listaB) == 0);
	TEST(; , __selftest_sortedBufList(listaB) == 1);

	TEST(; , allocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(; , totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(; , __selftest_sortedBufList(listaB) == 1);

	TEST(tab = (int*)getValueBlock(blockG) , tab != 0);
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

	TEST(deleteBlock(listaB, &blockD), allocatedMemorySizeList(listaB) == 88*sizeof(int));
	TEST(; , totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(deleteBlock(listaB, &blockA), allocatedMemorySizeList(listaB) == 86*sizeof(int));
	TEST(; , totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(; , tab[0] == 1);
	TEST(; , tab[1] == 1);
	TEST(; , tab[2] == 2);
	TEST(; , tab[3] == 3);
	TEST(; , tab[4] == 5);
	TEST(; , tab[5] == 8);
	TEST(; , tab[6] == 13);
	TEST(; , tab[7] == 21);
	TEST(; , tab[8] == 34);
	TEST(deleteBlock(listaB, &blockG), allocatedMemorySizeList(listaB) == 76*sizeof(int));
	TEST(; , totalAllocatedMemorySizeList(listaB) == 123*sizeof(int));
	TEST(deleteBlock(listaB, &blockB), allocatedMemorySizeList(listaB) == 54*sizeof(int));
	TEST(; , totalAllocatedMemorySizeList(listaB) == 88*sizeof(int));
	TEST(; , __selftest_sortedBufList(listaB) == 1);

	TEST(deleteBlock(listaB, &blockC), blockC == 0);
	TEST(deleteBlock(listaB, &blockE), blockC == 0);
	TEST(deleteBlock(listaB, &blockF), blockC == 0);
	TEST(deleteBlock(listaB, &blockO), blockO == 0);
	TEST(;, emptyList(listaB) == 1);
	TEST(; , __selftest_sortedBufList(listaB) == 1);

	TEST(deleteList(&listaB), listaB == 0);
	TEST(; , allocatedMemorySizeList(listaB) == 0);
	TEST(; , totalAllocatedMemorySizeList(listaB) == 0);
	TEST(; , emptyList(listaB) != 0);

	printf("%c[0;1mTesty: stop  ________________________________________________________[ %c[0;%dm%-3d/%3d%c[0;1m ]%c[0;0m\n",\
			0x1B, 0x1B, fail?41:42, ok, fail, 0x1B, 0x1B);

}

