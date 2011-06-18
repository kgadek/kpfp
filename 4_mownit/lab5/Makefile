CXX 		= g++
CXXFLAGS	= -W -Wall -Wshadow -Weffc++ -Wformat -Wsequence-point\
		   		-Wunused -Wuninitialized -Wfloat-equal -ansi -pedantic
LDFLAGS		= -O2 -lm -I/usr/tools/include -L/usr/tools/lib\
				-lgslcblas -lgsl -ggdb
SRC			= real_life.cpp
PSRC		= $(SRC) Makefile
PKGNAME 	= GadekKonrad-mownit5.7z
.PHONY: 	clean all pkg

all: $(SRC:%.cpp=%)

clean:
	rm -f *.o $(PKGNAME) *~ *swp $(SRC:%.cpp=%)

pkg: clean
	7za a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on $(PKGNAME) $(PSRC)

