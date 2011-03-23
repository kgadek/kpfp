CXX 		= g++
CXXFLAGS	= -W -Wall -Wshadow -Weffc++ -Wformat -Wsequence-point\
		   		-Wunused -Wuninitialized -Wfloat-equal -ansi -pedantic
LDFLAGS		= -O0 -ggdb -lm

SRC			= zad2A.cpp zad2B.cpp zad2C.cpp
.PHONY: 	clean all

all: $(SRC:%.cpp=%)

clean:
	rm -f *.o *~ *swp $(SRC:%.cpp=%) $(SRC:%.cpp=%_opt) $(SRC:%.cpp=%_opt1) core.*

