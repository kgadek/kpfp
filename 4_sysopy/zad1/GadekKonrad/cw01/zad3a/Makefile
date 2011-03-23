ifeq ($(KOMUNIKATY), 1)
	DOD	=	-D KOMUNIKATY
else
	DOD =
endif

vpath %.c src src/operacje src/reszta src/we_wy
vpath %.h src/include

NAME	=	macierze
DEF_IO	=	konsola
INCLUDE	=	-I src/include

CC		=	gcc
CFLAGS	=	-W -Wall $(INCLUDE) $(DOD)
LDFLAGS	=	-lm 

OBJS_COM=	macierz.o iloczyn.o roznica.o suma.o
OBJS_K	=	mainK.o wczytaj_konsola.o wypisz_konsola.o $(OBJS_COM)
OBJS_P	=	mainP.o wczytaj_plik.o wypisz_plik.o pliki.o $(OBJS_COM)

.PHONY: all clean

all: $(NAME)_$(DEF_IO)

clean:
	rm -f *.o *~ *swp $(NAME)

$(NAME)_konsola: $(OBJS_K)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o macierze

$(NAME)_pliki: $(OBJS_P)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o macierze

mainP.o: main.c			# specjalna reguła, bo main.o nie ma powiązanego pliku .h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $? -o $@ -D WEWY_PLIKI

mainK.o: main.c			# specjalna reguła, bo main.o nie ma powiązanego pliku .h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $? -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $? -o $@

