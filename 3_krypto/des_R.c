#include <stdio.h>
#include <openssl/des.h>
#include <string.h>

void ecb(FILE* inf, FILE* keyf, DES_key_schedule klucz, int tryb) {
	printf("\n--- dzialania w trybie ECB...\n");
	FILE* outf = fopen("plik.out", "w");
	int i=0;
	DES_cblock in, out;
	if(tryb) {	
		fwrite((void*)&klucz, sizeof(unsigned char), sizeof(DES_key_schedule), keyf);
		while((i=fread(&in, sizeof(unsigned char), 8, inf))==8) {
			DES_ecb_encrypt(&in, &out, &klucz, 1);
			fwrite(&out, sizeof(unsigned char), 8, outf);
		}
		if(i) {
			while(i<8)
				in[i++]='\0';
			DES_ecb_encrypt(&in, &out, &klucz, 1);
			fwrite(&out, sizeof(unsigned char), 8, outf);
		}
	} else {
		fread((void*)&klucz, sizeof(unsigned char), sizeof(DES_key_schedule), keyf);
		while((i=fread(&in, sizeof(unsigned char), 8, inf))==8) {
			DES_ecb_encrypt(&in, &out, &klucz, 0);
			fwrite(&out, sizeof(unsigned char), 8, outf);
		}
		if(i) {
			while(i<8)
				in[i++]='\0';
			DES_ecb_encrypt(&in, &out, &klucz, 0);
			fwrite(&out, sizeof(unsigned char), 8, outf);
		}
	}
	fclose(outf);
}
	
void cbc(FILE* inf, FILE* keyf, DES_key_schedule klucz, int tryb) {
	printf("\n--- dzialania w trybie CBC...\n");
	FILE* outf = fopen("plik.out", "w");
	int i=0;
	DES_cblock ivec, in, out;
	for(i=0; i<8; i++)
		ivec[i]='0';
	if(tryb)	{
		fwrite((void*)&klucz, sizeof(unsigned char), sizeof(DES_key_schedule), keyf);
		while((i=fread(&in, sizeof(unsigned char), 8, inf))==8) {
			DES_ncbc_encrypt(in, out, 8, &klucz, &ivec, 1);
			fwrite(&out, sizeof(unsigned char), 8, outf);
		}
	} else {
		fread((void*)&klucz, sizeof(unsigned char), sizeof(DES_key_schedule), keyf);
		while((i=fread(&in, sizeof(unsigned char), 8, inf))==8) {
			DES_ncbc_encrypt(in, out, 8, &klucz, &ivec, 0);
			fwrite(&out, sizeof(unsigned char), 8, outf);
		}
	}
	fclose(outf);
}

int main(int argc, char** args) {
	if(argc!=5 || (strcmp(args[1], "cbc") && strcmp(args[1], "ecb")) || (strcmp(args[2], "encrypt") && strcmp(args[2], "decrypt"))) {
		printf("uzycie: des cbc/ecb encrypt/decrypt plik.in plik.key\ndane wyjsciowe przekierowywane sa do pliku plik.out\n");
		return 1;
	}
	FILE* in = fopen(args[3], "r");
	if(!in) {
		printf("blad przy otwieraniu pliku...\n");
		return 1;
	}
	FILE* key;
	if(strcmp(args[2], "encrypt"))
		key = fopen(args[4], "r");
	else 
		key = fopen(args[4], "w");
	DES_key_schedule klucz;
	DES_cblock sekwencja;
	DES_random_key(&sekwencja);
	
	if(DES_set_key_checked(&sekwencja, &klucz)<0) {
		printf("blad przy generacji klucza!\n");
		return 1;
	}

	if(strcmp(args[1], "cbc")) {
		if(strcmp(args[2], "encrypt"))
			ecb(in, key, klucz, 0);
		else 
			ecb(in, key, klucz, 1);
	} else {
		if(strcmp(args[2], "encrypt"))
			cbc(in, key, klucz, 0); 
		else 
			cbc(in, key, klucz, 1);
	}
	fclose(in); fclose(key);
	return 0;
}
