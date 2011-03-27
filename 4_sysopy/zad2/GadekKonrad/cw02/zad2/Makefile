MAC_NAME	= macierze
COR_NAME	= corbusier
COR_PATH	= ../zad1
TMPSUFFIX	= .tmp

CC 			= gcc
CFLAGS		= -O0 -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self -Wunsafe-loop-optimizations\
				-Wunused -Wfloat-equal -Wmissing-include-dirs -Wcast-qual -Wwrite-strings -Winline\
				-Wconversion -Wjump-misses-init -Wlogical-op -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long -Wstack-protector -I. -I$(COR_PATH)
LDFLAGS		= -ggdb

LIBSRC		= macierze.c
.PHONY: 	all clean lib_$(COR_NAME)

all: lib_$(COR_NAME) lib_$(MAC_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=1 main.c tests.c -o main-st -Lst -l$(MAC_NAME) -l$(COR_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=2 main.c tests.c -o main-sh -Lsh -l$(MAC_NAME) -l$(COR_NAME) -Wl,-rpath,sh
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=3 main.c tests.c -o main-dl -Lsh -l$(MAC_NAME) -l$(COR_NAME) -ldl -Lsh -Wl,-rpath,sh

dirstruct:
	mkdir -p st sh

lib_$(MAC_NAME): lib_$(MAC_NAME)_st lib_$(MAC_NAME)_sh

lib_$(COR_NAME): dirstruct
	make static shared -C $(COR_PATH)
	cp $(COR_PATH)/st/lib$(COR_NAME).a st
	cp $(COR_PATH)/sh/lib$(COR_NAME).so sh

lib_$(MAC_NAME)_st: $(LIBSRC:%.c=%.o) dirstruct
	ar -r st/lib$(MAC_NAME).a $(LIBSRC:%.c=%.o)

lib_$(MAC_NAME)_sh: $(LIBSRC:%.c=%.dl.o) dirstruct
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -Wl,-soname,sh/lib$(MAC_NAME).so -o sh/lib$(MAC_NAME).so $(LIBSRC:%.c=%.dl.o)

clean:
	rm -rf st sh *.o *.so *.a *~ *swp .*swp main-* *.tmp core.*

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@ -Lsh -l$(COR_NAME)

%.dl.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) -fPIC -c $< -o $@

tests: all
	./main-dl
	./main-sh
	./main-st

