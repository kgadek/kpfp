CC 			= gcc
CFLAGS		= -O2 -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self\
				-Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline\
				-Wstack-protector -Wunsafe-loop-optimizations -Wlogical-op -Wjump-misses-init -Wmissing-include-dirs\
				-Wconversion -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long
LDFLAGS		= -lm

.PHONY: 	clean all

all: testerS testerL

testerS: tester.c tester.h
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -D SYS=1

testerL: tester.c tester.h
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -D SYS=0

clean:
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* testerL testerS *.out

