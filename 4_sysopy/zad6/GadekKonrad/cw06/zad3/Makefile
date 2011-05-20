CC 			= gcc
CFLAGS		= -O2 -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self\
				-Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline\
				-Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op -Wjump-misses-init -Wmissing-include-dirs\
				-Wconversion -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long
LDFLAGS		= -lm

SRC			= piper.c
.PHONY: 	clean all

all: $(SRC:%.c=%)
	mkfifo 12.pipe
	mkfifo 23.pipe
	mkfifo 34.pipe
	mkfifo 45.pipe
	mkfifo 51.pipe

clean:
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.c=%) *.out *.pipe
