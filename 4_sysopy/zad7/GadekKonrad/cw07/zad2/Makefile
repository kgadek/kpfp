CC 			= gcc
CFLAGS		= -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self\
				-Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline\
				-Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op -Wjump-misses-init -Wmissing-include-dirs\
				-Wconversion -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long
LDFLAGS		= -lm -lrt

SRC			= serwer.c klient.c
.PHONY: 	clean all

all: $(SRC:%.c=%) 

serwer: common.o
klient: common.o

clean:
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.c=%) *.out
