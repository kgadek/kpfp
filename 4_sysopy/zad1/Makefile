NAME		= GadekKonrad
EXERCISE	= 01

PKGNAME 	= $(NAME)-cw$(EXERCISE).tar.gz
.PHONY: 	clean pkg dirstruct

pkg: clean
	tar -czvf $(PKGNAME) $(NAME) 

clean:
	rm -f $(PKGNAME)
	find `pwd` -mindepth 2 -iname Makefile -type f | sed 's:\(.*\)/.*:\1:g' | xargs -n1 make clean -C

dirstruct:
	mkdir -p $(NAME)/cw$(EXERCISE)

