CXX			=	clang++
CXXFLAGS	=	-march=native -ansi -pedantic -O2 -g -W \
				-Wall -Wextra -Wshadow -Wformat -Winit-self \
				-Wunused -Wfloat-equal -Wcast-qual \
				-Wwrite-strings -Winline -Wstack-protector \
				-Wmissing-include-dirs -Wconversion \
				-Wmissing-prototypes -Wmissing-declarations
LDFLAGS		=	-lm -L/usr/tools/lib

PSRC		=	proto.py
CSRC		=	proto.cpp
.PHONY:		clean all all_py all_c

all: all_c
	cat .gitignore | sort | uniq > .gitignore.tmp
	rm .gitignore
	mv .gitignore.tmp .gitignore

all_c: $(CSRC:%.cpp=%)
	echo $? | sed 's/ /\n/g' >> .gitignore


%: %.cpp
	$(CXX) $(CXXFLAGS) $? -c -o $@

clean:
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.cpp=%) *.out
