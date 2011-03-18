/* made by Sandra Imiela & jerzy Muller (partially ;P) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/blowfish.h>

#define CBC 0
#define ECB 1

void syntaxErr() 
{
 printf("Nieprawidlowa skladnia!/n wzor: {-enc|-dec} {-cbc|-ecb} plik_wej plik_wyj haslo [wektor]/n wektor podajemy tylko w trybie CBC, jeśli nie zostanie podany to jest inicjalizowany zerami/n");
}

int main(int argc, char* argv[])
{
 //parsowanie linii polecen
 if (argc != 6 && argc != 7) 
    {
	 syntaxErr();
	 return 1;
	}
 //tryb zapis/odczyt
 int tryb;
 if (strcmp (argv[1],"-enc") == 0) 
    tryb = BF_ENCRYPT;
 else
     if (strcmp (argv[1],"-dec") == 0) 
        tryb = BF_DECRYPT;
  	 else 
          {
           syntaxErr();
           return 1;
          }
 //tryb CBC/ECB
 int opmode;
 if (strcmp (argv[2],"-cbc") == 0) 
    opmode = CBC;
 else
	 if (strcmp (argv[2],"-ecb") == 0) 
        opmode = ECB;
	 else
         {
          syntaxErr();
          return 1;
         }
 
 if (opmode == ECB && argc == 7) 
    {
	 syntaxErr(argv[0]);
	 return 1;
    }
 
 //tworzymy sobie klucz
 BF_KEY klucz;
 BF_set_key(&klucz, strlen(argv[5]), argv[5]);

 unsigned char wektor[8];

 //otwieramy plik wejsciowy
 FILE * fin = fopen(argv[3], "rb");
 if (fin == NULL) 
    {
	 printf("Blad podczas otwierania pliku wejsciowego!/n");
	 return 1;
    }
 
 //wektor poczatkowy - czytamy lub tworzymy
 if (opmode == CBC) 
    {
     if (argc == 7) 
        {
		 FILE * wektorf = fopen(argv[6], "rb");
         if (wektorf == NULL) 
            {
             printf("Blad podczas otwierania wektora poczatkowego!/n");
             fclose(fin);
             return 1;
            }
         fread(wektor, sizeof(wektor), 1, wektorf);
		 fclose(wektorf);
        } 
     else
         memset(wektor, 0, sizeof(wektor));
    }
 
 //otwieramy plik do zapisu
 FILE * fout = fopen(argv[4], "wb");
 if (fout == NULL)
    {
	 fclose(fout);
	 printf("Blad podczas otwierania pliku wyjsciowego!/n");
	 return 1;
    }
 
 //szyfrujemy
 while (1)
       {
	    unsigned char in[8], out[8];
	    size_t bytes = fread(in, 1, sizeof(in), fin);
        
	    if (bytes == 0) break;
	    size_t i;
        if (bytes < sizeof(in))
		   for (i = bytes; i < sizeof(in); i++)
			   in[i] = 0;
        if (opmode == ECB)
			BF_ecb_encrypt(in, out, &klucz, tryb);
        else
		    BF_cbc_encrypt(in, out, sizeof(in), &klucz, wektor, tryb);
        
        if (tryb == BF_DECRYPT) 
           {
	       	bytes = 0;
		    while (out[bytes] != 0 && bytes < sizeof(in)) bytes++;
           }
    	else bytes = sizeof(in);
    	fwrite(out, bytes, 1, fout);
       }
 
 fclose(fin);
 fclose(fout);
 return 0;
}
