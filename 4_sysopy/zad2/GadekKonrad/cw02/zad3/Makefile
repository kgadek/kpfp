COR_NAME	= corbusier
COR_PATH	= ../zad1
MAC_NAME	= macierze
MAC_PATH	= ../zad2

CC 			= gcc
CFLAGS		= -O0 -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self -Wunsafe-loop-optimizations\
				-Wunused -Wfloat-equal -Wmissing-include-dirs -Wcast-qual -Wwrite-strings -Winline\
				-Wconversion -Wjump-misses-init -Wlogical-op -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long -Wstack-protector -I. -I$(COR_PATH) -I$(MAC_PATH)
LDFLAGS		= -ggdb

.PHONY: 	all clean

all: tests

dirstruct:
	mkdir -p st sh

lib_$(MAC_NAME): lib_$(COR_NAME) dirstruct
	cp $(MAC_PATH)/st/lib$(MAC_NAME).a st
	cp $(MAC_PATH)/sh/lib$(MAC_NAME).so sh

lib_$(COR_NAME): dirstruct
	make -C $(MAC_PATH)
	cp $(MAC_PATH)/st/lib$(COR_NAME).a st
	cp $(MAC_PATH)/sh/lib$(COR_NAME).so sh

clean:
	rm -rf st sh *.o *.so *.a *~ *swp .*swp *.tmp core.* tester-*

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

tests: tests-mac tests-cor

tests-mac: lib_$(COR_NAME) lib_$(MAC_NAME) build-tests-mac
	./tester-mac-st
	./tester-mac-sh
	./tester-mac-dl

tests-cor: lib_$(COR_NAME) build-tests-cor
	./tester-cor-st
	./tester-cor-sh
	./tester-cor-dl

build-tests-mac: tester_common.o  
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=1 tester_mac.c tests_mac.c tester_common.o -o tester-mac-sh -Lst -l$(MAC_NAME) -l$(COR_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=2 tester_mac.c tests_mac.c tester_common.o -o tester-mac-st -Lsh -l$(MAC_NAME) -l$(COR_NAME) -Wl,-rpath,sh
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=3 tester_mac.c tests_mac.c tester_common.o -o tester-mac-dl -Lsh -l$(MAC_NAME) -l$(COR_NAME) -ldl -Lsh -Wl,-rpath,sh

build-tests-cor: tester_common.o
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=1 tester_cor.c tests_cor.c tester_common.o -o tester-cor-sh -Lst -l$(COR_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=2 tester_cor.c tests_cor.c tester_common.o -o tester-cor-st -Lsh -l$(COR_NAME) -Wl,-rpath,sh
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=3 tester_cor.c tests_cor.c tester_common.o -o tester-cor-dl -Lsh -l$(COR_NAME) -ldl -Lsh -Wl,-rpath,sh
