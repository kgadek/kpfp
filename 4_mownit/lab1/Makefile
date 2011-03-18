CXX 		= g++
CXXFLAGS	= -W -Wall -Wshadow -Weffc++ -Wformat -Wsequence-point\
		   		-Wunused -Wuninitialized -Wfloat-equal -ansi -pedantic
LDFLAGS		= -O2 -lm -I/usr/tools/include -L/usr/tools/lib\
				-lgslcblas -lgsl
SRC			= zad1.cpp zad2.cpp zad3.cpp zad4.cpp zad5.cpp
PSRC		= $(SRC) Makefile
PKGNAME 	= GadekKonrad-mownit1.7z
.PHONY: 	clean all pkg

all: $(SRC:%.cpp=%)

clean:
	rm -f *.o $(PKGNAME) *~ *swp $(SRC:%.cpp=%)

pkg: clean
	7za a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on $(PKGNAME) $(PSRC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $? -c -o $@

