SRC		= 
CXXSRC	= val

.PHONY:	all clean tags pkg

all: $(SRC:%=%.bla) $(CXXSRC:%=%.ble) tags
	cat .gitignore | sort | uniq > .gitignore.tmp
	rm .gitignore
	mv .gitignore.tmp .gitignore

clean:
	rm -f *.yy.c *.tab.c *.tab.h $(SRC) $(CXXSRC) TAGS *.tar

pkg: $(SRC:%=%.y) $(SRC:%=%.l) $(CXXSRC:%=%.y) $(CXXSRC:%=%.l) Makefile
	tar -cf gadek.tar $?

tags: $(SRC:%=%.bla) $(CXXSRC:%=%.ble)
	etags *.c *.h

%.bla: %.tab.c %.yy.c 
	echo $(@:%.bla=%) >> .gitignore
	gcc -o $(@:%.bla=%) $?

%.ble: %.tab.c %.yy.c
	echo $(@:%.ble=%) >> .gitignore
	g++ -o $(@:%.ble=%) $?

%.yy.c: %.l
	flex -o $@ $? 

%.tab.c: %.y
	bison -d $?

