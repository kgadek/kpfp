CXX 		= g++
CXXFLAGS	= -W -Wall -Wshadow -Weffc++ -Wformat -Wsequence-point\
		   		-Wunused -Wuninitialized -Wfloat-equal -ansi -pedantic
LDFLAGS		= 

ZAD1AFLAGS	= -O3
ZAD1BFLAGS	= -O2
ZAD1CFLAGS	= -O1 -foptimize-sibling-calls
ZAD1DFLAGS	= -O1 -fomit-frame-pointer -fregmove -fira-loop-pressure -fipa-struct-reorg -fwhole-program

SRC			= zad1A.cpp zad1B.cpp zad1C.cpp zad1D.cpp
.PHONY: 	clean all tests

all: $(SRC:%.cpp=%) $(SRC:%.cpp=%_opt) $(SRC:%.cpp=%_opt1)

clean:
	rm -f *.o *~ *swp $(SRC:%.cpp=%) $(SRC:%.cpp=%_opt) $(SRC:%.cpp=%_opt1)

tests: all $(SRC:%.cpp=tests-%)

tests-%: $(@:tests-%=%)
	@echo ////////////////////////////////////[ $(@:tests-%=%) ]///////////////////////////////////
	@echo Niezoptymalizowane______________________________________________________________
	@time -p ./$(@:tests-%=%) > /dev/null
	@echo Zoptymalizowane -O1_____________________________________________________________
	@time -p ./$(@:tests-%=%)_opt1 > /dev/null
	@echo Zoptymalizowane dodatkowo_______________________________________________________
	@time -p ./$(@:tests-%=%)_opt > /dev/null

%: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $? -o $@

%_opt1: %.cpp
	$(CXX) $(CXXFLAGS) -O1 $(LDFLAGS) $? -o $@

zad1A_opt: zad1A.cpp
	$(CXX) $(CXXFLAGS) $(ZAD1AFLAGS) $(LDFLAGS) $? -o $@

zad1B_opt: zad1B.cpp
	$(CXX) $(CXXFLAGS) $(ZAD1BFLAGS) $(LDFLAGS) $? -o $@

zad1C_opt: zad1C.cpp
	$(CXX) $(CXXFLAGS) $(ZAD1CFLAGS) $(LDFLAGS) $? -o $@

zad1D_opt: zad1D.cpp
	$(CXX) $(CXXFLAGS) $(ZAD1DFLAGS) $(LDFLAGS) $? -o $@

