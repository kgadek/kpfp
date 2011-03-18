SRC			= spr.docx rownanieKonwekcjiDyfuzjiFDM.m eliminacjaGaussa.m rownaniePrzewodnictwaCieplaFDM.m Main.m
PKGNAME		= 2_2_Gadek_Konrad

.PHONY: all clean pkg

all: pkg
clean:
	rm -rf $(PKGNAME)
	rm -f $(PKGNAME).zip
	rm -f *.o *~ *swp zad1
pkg: clean
	mkdir $(PKGNAME)
	cp $(SRC) $(PKGNAME)
	zip -r $(PKGNAME).zip $(PKGNAME)
	rm -rf $(PKGNAME)
	rm -rf $(PKGNAME)

