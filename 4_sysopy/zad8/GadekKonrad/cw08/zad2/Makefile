CC 			= gcc
CFLAGS		= -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self\
				-Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline\
				-Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op -Wjump-misses-init -Wmissing-include-dirs\
				-Wconversion -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long
LDFLAGS		= -lm -ggdb -lrt -pthread

SRC			= pisarz.c czytelnik.c
.PHONY: 	clean all ipcclean

all: $(SRC:%.c=%)

pisarz czytelnik: common.o

clean: ipcclean
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.c=%) *.out

ipcclean:
	ipcs | grep 2588 | awk '{print -m  ;}' | xargs ipcrm 2&>/dev/null
	ipcs | egrep '755[ 	]*3' | awk '{print -s  ;}' | xargs ipcrm 2&>/dev/null
