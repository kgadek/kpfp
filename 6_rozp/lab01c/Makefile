CC 			= clang
CFLAGS		= -ansi -pedantic -W -Wall -Wextra -Wshadow -Wformat -Winit-self\
				-Wunused -Wfloat-equal -Wcast-qual -Wwrite-strings -Winline\
				-Wstack-protector -Wmissing-include-dirs -Wconversion\
				-Wmissing-prototypes -Wmissing-declarations -Wno-long-long
LDFLAGS		= -lm -ggdb -lpthread

SRC			= tcpclient.c tcpserver.c
.PHONY: 	clean all

all: $(SRC:%.c=%)

clean: 
	rm -rf *.o *.so *.a *~ *swp .*swp *.tmp core.* $(SRC:%.c=%) *.out

