#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

int main(int argc, char** argv) {
	char * filename = "deskey.out";
	if(argc>=2)
		filename = argv[1];
	
	FILE * fout = fopen(filename, "wb");
	if(fout == NULL) {
		printf("Nie udalo sie otworzyc pliku wyjsciowego!\n");
		return 1;
	}
	
	DES_cblock key;
	DES_random_key(&key);
	
	if(fwrite(&key, sizeof(key), 1, fout) < 1) {
		printf("Nie udalo sie zapisac do pliku!\n");
		return 1;
	}
	
	fclose(fout);	
	return 0;
}

