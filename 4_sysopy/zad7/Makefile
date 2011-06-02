NAME		= GadekKonrad
EXERCISE	= 07

EXCLUDEFILE	= .exclude
PKGNAME 	= $(NAME)-cw$(EXERCISE).tar.gz
.PHONY: 	clean pkg dirstruct

pkg: clean
	find $(NAME) -type f -iname '.*project' > $(EXCLUDEFILE)
	find $(NAME) -type f -iname '.gitignore' >> $(EXCLUDEFILE)
	tar -czvhf $(PKGNAME) -X $(EXCLUDEFILE) $(NAME)
	rm -f $(EXCLUDEFILE)

clean:
	rm -f $(PKGNAME)
	find `pwd` -mindepth 2 -iname Makefile -type f | sed 's:\(.*\)/.*:\1:g' | xargs -r -n1 make clean -C

dirstruct:
	mkdir -p $(NAME)/cw$(EXERCISE)

