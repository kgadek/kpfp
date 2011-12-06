CXX			=	clang++
CXXFLAGS	=	-march=native -std=c++98 -pedantic -O2 -g
#CXXFLAGS	=	-O2 -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self -Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline -Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op -Wjump-misses-init -Wmissing-include-dirs -Wconversion -Wmissing-prototypes -Wmissing-declarations
LDFLAGS		=	-lm -L/usr/tools/lib

TSRC		=	test.NewHandlerSupport.cpp
SRC			=	testio.cpp $(TSRC)
.PHONY:		clean all all_app

all: all_app $(OSRC:%.cpp=%.o) $(TSRC:%.cpp=%)

all_app: $(SRC:%.cpp=%)
	echo $? | sed 's/ /\n/g' > .gitignore

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $? -c -o $@

clean:
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.cpp=%) *.out

