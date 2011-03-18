// Program: generowanie klucza prywatnego i publicznego algorytmem dsa512
// manuale z ktorych korzystam:
// http://www.digipedia.pl/man/d2i_DSAPrivateKey.3.html
// http://www.openssl.org/docs/crypto/bn.html
// http://www.openssl.org/docs/crypto/BIO_s_file.html
// http://www.openssl.org/docs/crypto/BN_bn2bin.html
#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<openssl/rsa.h>
#include<openssl/err.h>
#include <openssl/evp.h>
#include<openssl/md5.h>
#include<openssl/bio.h>

int fileSize(BIO *b)
{
	FILE* plik;
  BIO_get_fp(b,&plik);
	int size;
	fseek(plik,0L, SEEK_END);
	size =  ftell(plik);
	fseek(plik,0L, SEEK_SET);
	return size;
}
int main(int argv, char** args)
{
	if(argv < 3) { printf("podales za malo argumentow( argument 1 - plik podpisany, argument 2 - plik z podpisem\n"); exit(1); }
	// inicjalizacja:
	int n;
	MD5_CTX md5_ctx;
	unsigned char* signature_bufor;
	unsigned char* binary_key;
	unsigned char hash_bufor[128];
	unsigned char hash[16];
	const unsigned char *p;
	BIO *in;
	char error_buf[122];
	int error;
	printf("tworzenie struktury dsa...                   ");
	RSA* rsa = NULL;// = NULL; //DSA_new(); // inicjalizacja struktury dsa
	error = ERR_get_error(); if(error)printf("wystapil blad nr %d\n",error); else printf("OK\n");
	
	// odczyt pliku z kluczem publicznym
	printf("otwarcie pliku z kluczem prywatnym...        ");
	in = BIO_new_file("pub_key.rsa", "rb");
	if(!in) printf("wystaplil blad w otwieraniu pliku\n"); else printf("OK\n");
	int key_size = fileSize(in);
	printf("rozmiar otworzonego pliku: %d\n",key_size);
	printf("odczyt klucza z pliku pub_key.rsa...         ");
	binary_key = OPENSSL_malloc(key_size);
	BIO_read(in, binary_key, key_size);
  int i;
//	printf("tak wyglada otwarty klucz:                   ");
//	printf("\n"); 	for(i = 0 ; i < key_size ; ++i) printf("%c",binary_key[i]); 	printf("\n");
	error = ERR_get_error(); if(error)printf("wystapil blad nr %d\n",error); else printf("OK\n");
	printf("tworzenie struktury dsa na podstawie klucza...OK\n");
	p = binary_key;
	d2i_RSAPublicKey(&rsa, &p, key_size);
	if(rsa == NULL) printf("blad w tworzeniu struktury rsa\n");
	BIO_free(in);

	// tworzenie hashu pliku z wiadomoscia
  printf("otwarcie pliku z wiadomoscia...              ");
	in = BIO_new_file(args[1],"r");
	if(!in) printf("wystaplil blad w otwieraniu pliku\n"); else printf("OK\n");
	int file_size = fileSize(in);
	printf("rozmiar otworzonego pliku: %d\n",file_size);
	
	printf("tworzenie hashu pliku z wiadomoscia...       ");

	MD5_Init(&md5_ctx);
	while ((n = BIO_read(in, hash_bufor, sizeof(hash_bufor)))>0)
			MD5_Update(&md5_ctx, hash_bufor, n);
	MD5_Final(hash, &md5_ctx);
	printf("OK\n");
	
	printf("tak wyglada hash pliku:\n");
	for(n = 0; n < sizeof(hash); n++) 
		printf("%02x ", hash[n]);
	printf("\n");

  // wczytywanie podpisu pliku
 	printf("otwarcie pliku z podpisem...                 ");
	in = BIO_new_file(args[2], "rb");
	if(!in) printf("wystaplil blad w otwieraniu pliku\n"); else printf("OK\n");
	int signature_size = fileSize(in);
	printf("rozmiar otworzonego pliku: %d\n",signature_size);
	printf("odczyt podpisu z pliku 'signature'...        ");
	signature_bufor = OPENSSL_malloc(signature_size);
	BIO_read(in, signature_bufor, signature_size);
	printf("OK\n");
	int wynik = RSA_verify(NID_md5, hash, sizeof(hash), signature_bufor, signature_size, rsa);
	if(wynik == 1)
		printf("weryfikacja zakonczona - sygnatura prawidlowa\n");
	else if(wynik == 0)
		printf("weryfikacja zakonczona - zla sygnatura\n");
	else
		printf("blad weryfikacji\n");
		
	// sprzatamy
	RSA_free(rsa);			// czyscimy pamiec po strukturze dsa
  return 0;
}
