EXERCISE	= 01
NAME		= Gadek_Konrad_$(EXERCISE)

EXCLUDEFILE	= .exclude
PKGNAME 	= $(NAME).tgz
.PHONY: 	clean pkg dirstruct

pkg: clean
	find $(NAME) -type f -iname '.*project' > $(EXCLUDEFILE)
	find $(NAME) -type f -iname '.gitignore' >> $(EXCLUDEFILE)
	find $(NAME) -type d -iname '*_ext_*' >> $(EXCLUDEFILE)
	tar -czvhf $(PKGNAME) -X $(EXCLUDEFILE) $(NAME)
	rm -f $(EXCLUDEFILE)

clean:
	rm -f $(PKGNAME)
	find `pwd` -mindepth 2 -iname Makefile -type f | sed 's:\(.*\)/.*:\1:g' | xargs -r -n1 make clean -C

dirstruct:
	mkdir -p $(NAME)/cw$(EXERCISE)

