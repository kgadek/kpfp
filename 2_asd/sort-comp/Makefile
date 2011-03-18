sortcomp.o: sortcomp.cpp
	g++ -c sortcomp.cpp -o sortcomp.o
test.o: test.cpp
	g++ -c test.cpp -o test.o
cmbsort.o: cmbsort.cpp
	g++ -c cmbsort.cpp -o cmbsort.o
bblsort.o:
	g++ -c bblsort.cpp -o bblsort.o
all: sortcomp.o test.o cmbsort.o bblsort.o
	g++ sortcomp.o test.o cmbsort.o bblsort.o -o sortcomp
