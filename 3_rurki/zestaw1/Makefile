CXX			= g++
CXXFLAGS	= -W -Wall -Wshadow -Wformat -Wsequence-point -Wunused\
		-Wuninitialized -Wfloat-equal -ansi -pedantic
LDFLAGS		= -O2 -lm
SRC			= doc.latex zad1.cpp doc.pdf
PKGNAME		= 1_2_Gadek_Konrad

.PHONY: all clean latexclean pkgclean doc zad1 pkg

all: zad1 doc
allclean: clean latexclean pkgclean
	rm -f doc.pdf
pkgclean:
	rm -rf $(PKGNAME)
	rm -f $(PKGNAME).zip
clean:
	rm -f *.o *~ *swp zad1
latexclean:
	rm -f *.aux *.log *.toc *.ilg *.ind
doc: doc.latex
	pdflatex doc.latex
zad1: zad1.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $? -o $@
pkg: allclean
	pdflatex doc.latex
	pdflatex doc.latex
	pdflatex doc.latex
	pdflatex doc.latex
	pdflatex doc.latex
	mkdir $(PKGNAME)
	cp $(SRC) $(PKGNAME)
	zip -r $(PKGNAME).zip $(PKGNAME)
	rm -rf $(PKGNAME)
	make latexclean
	make clean

