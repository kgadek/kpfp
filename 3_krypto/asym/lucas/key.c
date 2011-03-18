// Program: generowanie klucza prywatnego i publicznego algorytmem dsa512
// manuale (niektrore!)  z ktorych korzystam:
// http://www.openssl.org/docs/crypto/bn.html
// http://www.openssl.org/docs/crypto/BIO_s_file.html
// http://www.openssl.org/docs/crypto/BN_bn2bin.html
#include<stdio.h>
#include<openssl/dsa.h>
#include<openssl/err.h>
#include<openssl/bio.h>
int main()
{
	int bits = 512;
	int bytes = 64;
	//unsigned char binary_key[bytes+1];  // 512 bitow, czyli 64 bajty
	unsigned char* bufor = NULL;
	unsigned char* p;
		// 1: generowanie struktury DSA
	int error;
	printf("tworzenie struktury dsa...                   ");
	DSA* dsa = DSA_new(); // inicjalizacja struktury dsa
	error = ERR_get_error(); if(error)printf("wystapil blad nr %d\n",error); else printf("OK\n");
	printf("inicjalizacja struktury dsa...               ");
	dsa = DSA_generate_parameters(1024,NULL,0,NULL,NULL,NULL,NULL);
	error = ERR_get_error(); if(error)printf("wystapil blad nr %d\n",error); else printf("OK\n");
	// 2: generowanie pary kluczy
  printf("generowanie klucza...                        ");
  DSA_generate_key(dsa);
	error = ERR_get_error(); if(error)printf("wystapil blad nr %d\n",error); else printf("OK\n");
  // 3: zapisywanie kluczy do plikow
	// b): klucza prywatnego
	//FILE* out1 = fopen("priv_key.dsa", "wb");
	BIO *out;
	printf("tworzenie pliku z kluczem prywatnym ...      ");
	out = BIO_new_file("priv_key.dsa", "wb");
	if(!out) printf("wystaplil blad w tworzeniu pliku\n"); else printf("OK\n");
	printf("konwersja do postaci binarnej...             ");
//	BN_bn2bin(dsa->priv_key, binary_key);
	bufor = NULL;
	dsa->write_params = 1;
	int size = i2d_DSAPrivateKey(dsa, NULL);
	bufor = OPENSSL_malloc(size);
	p = bufor;
	i2d_DSAPrivateKey(dsa, &bufor);
	printf("OK\n");
	printf("rozmiar pliku binarnego z kluczem prywatnym: %d\n",size);
	printf("zapis do pliku pub_key.dsa...                ");
	BIO_write(out, p, size);
//	fwrite(p, size, 1, out1);
	
	printf("OK\n");
	OPENSSL_free(p);
	BIO_free(out);
	// a): klucza publicznego
	printf("tworzenie pliku z kluczem publicznym...      ");
	out = BIO_new_file("pub_key.dsa", "wb");
	if(!out) printf("wystaplil blad w tworzeniu pliku\n"); else printf("OK\n");
	printf("konwersja do postaci binarnej DER...         ");
	bufor = NULL;
	//BN_bn2bin(dsa->pub_key, binary_key);
	size = i2d_DSAPublicKey(dsa, NULL);
	bufor = OPENSSL_malloc(size);
	p = bufor;
	i2d_DSAPublicKey(dsa, &bufor);
	printf("OK\n");
	printf("rozmiar pliku binarnego z kluczem publicznym: %d\n",size);
	printf("zapis do pliku pub_key.dsa...                ");
	BIO_write(out, p, size);
	printf("OK\n");
	OPENSSL_free(p);
	BIO_free(out);//*/
	// 4: porzadki:
	printf("zwalnianie pamieci po strukturze dsa...      ");
	DSA_free(dsa);			// czyscimy pamiec po strukturze dsa
	error = ERR_get_error(); if(error)printf("wystapil blad nr %d\n",error); else printf("OK\n");
	return 0;
}
