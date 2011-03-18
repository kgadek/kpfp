OWNER       = GadekKonrad
PROJECT     = ref02
SRC         = ex_says.c ex_comb.c ex_inf.c
PSRC        = $(SRC) Makefile pytania.txt index.html style.css materialy

CC          = gcc
CPPFLAGS    = -W -Wall -Wshadow -Wformat -Wsequence-point \
              -Wunused -Wuninitialized -Wfloat-equal -ansi -pedantic
LDFLAGS     = -ggdb
PKGROOT     = $(OWNER)/$(PROJECT)
PKGNAME     = $(OWNER)-$(PROJECT).tar.gz
.PHONY:     clean all pkg

all: $(SRC:%.c=%)

clean:
	rm -rf *.o *~ *swp $(SRC:%.c=%) $(PKGROOT) $(PKGNAME)

pkg: clean
	rm -rf $(OWNER)
	mkdir -p $(PKGROOT)
	cp -R $(PSRC) $(PKGROOT)
	tar -czf $(PKGNAME) $(PKGROOT)
	rm -rf $(OWNER)

