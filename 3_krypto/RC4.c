/* made by Sandra Imiela & jerzy Muller (partially ;P) */

#include <stdio.h>
#include <openssl/rc4.h> 
#include <string.h>

int main (int argc, char* argv[])
{
 char nazwa[30];
 FILE *fpin,*fpout;
 unsigned char in[16],out[16];
 RC4_KEY klucz;
 int dlugosc = 16;
 int i, j;

 if (argc != 3) 
    {
     printf ("Blad! \n Poprawne uruchomienie: klucz plik_tekstowy \n");
	 return 1;
    }

 strcpy(nazwa,argv[2]);
 printf("%s\n",nazwa);
 fpin = fopen(nazwa, "r");	// otwarcie pliku do czytania

 // tworzenie pliku wyjsciowego
 i = 0;
 while (nazwa[i] != '.' && i < strlen(nazwa)) i++;
 strcpy(nazwa+i, "RC4.txt");

 fpout = fopen(nazwa, "w");
 
 // tworzenie klucza RC4
 RC4_set_key(&klucz, dlugosc, argv[1]);
 
 // czytanie pliku wejsciowego i szyfrowanie
 while (i = fread(in, 1, 16, fpin))
       {
        RC4(&klucz, i, in, out);
        fwrite(out, 1, i, fpout);
       }
 fclose(fpin);
 fclose(fpout);
 
 return 0;
}

