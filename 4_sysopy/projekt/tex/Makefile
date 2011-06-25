NAME		=	mozdzer
MAINSRC		=	$(NAME).tex
BIBAUX		=	$(NAME).aux

.PHONY: all clean

DIRT		=	*.aux *.lof *.log *.lot *.out *.toc *.blg *.bbl

all: *.tex
	pdflatex $(MAINSRC)
	bibtex $(BIBAUX)
	pdflatex $(MAINSRC)
	pdflatex $(MAINSRC)

clean:
	rm -f $(DIRT)

distclean: clean
	rm -f *.pdf

