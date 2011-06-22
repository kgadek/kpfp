CC 			= gcc
CFLAGS		= -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self\
				-Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline\
				-Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op\
				-Wjump-misses-init -Wmissing-include-dirs -Wconversion\
				-Wmissing-prototypes -Wmissing-declarations -Wno-long-long
LDFLAGS		= -lm -ggdb -lpthread

SRC			= client.c
.PHONY: 	clean all ipcclean

all: $(SRC:%.c=%)

clean: 
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.c=%) *.out

