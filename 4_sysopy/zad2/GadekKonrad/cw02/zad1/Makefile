LIBNAME		= corbusier
TMPSUFFIX	= .tmp

CC 			= gcc
CFLAGS		= -O0 -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self -Wunsafe-loop-optimizations\
				-Wunused -Wfloat-equal -Wmissing-include-dirs -Wcast-qual -Wwrite-strings -Winline\
				-Wconversion -Wjump-misses-init -Wlogical-op -Wmissing-prototypes -Wmissing-declarations\
				-Wno-long-long -Wstack-protector -I.
LDFLAGS		= -ggdb -lm

LIBSRC		= corbusier.c
.PHONY: 	clean all

all: static shared
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=1 main.c tests.c -o main-st -Lst -l$(LIBNAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -D LIBTYPE=2 main.c tests.c -o main-sh -Lsh -l$(LIBNAME) -Wl,-rpath,sh

dirstruct:
	mkdir -p st sh

static: $(LIBSRC:%.c=%.o) dirstruct
	ar -r st/lib$(LIBNAME).a $(LIBSRC:%.c=%.o)

shared: $(LIBSRC:%.c=%.dl.o) dirstruct
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -Wl,-soname,lib$(LIBNAME).so -o sh/lib$(LIBNAME).so $(LIBSRC:%.c=%.dl.o)
	
%.dl.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) -fPIC -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

clean:
	rm -rf st sh *.o *.so *.a *~ *swp .*swp main-* *.tmp core.*

