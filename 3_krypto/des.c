#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

#define ECB 0
#define CBC 1

int main(int argc, char** argv) {
	FILE *fkey, *fin, *fout;
	int enc, mode;
	
	if(argc!=6) {
		printf("Blad skladni!\n");
		return 1;
	}
	
	fin = fopen(argv[1], "r");
	if(fin == NULL) {
		printf("Blad przy otwieraniu pliku wejsciowego!\n");
		return 1;
	}
	
	fout = fopen(argv[2], "w");
	if(fout == NULL) {
		printf("Blad przy otwieraniu pliku wyjsciowego!\n");
		return 1;
	}
	
	fkey = fopen(argv[3], "r");
	if(fkey == NULL) {
		printf("Blad przy otwieraniu pliku klucza!\n");
		return 1;
	}
	
	DES_cblock key;
	if(fread(&key, sizeof(key), 1, fkey) < 1) {
		printf("Blad przy wczytywaniu klucza!\n");
		return 1;
	}
	fclose(fkey);
	
	if(strcmp(argv[4], "-enc")==0)
		enc = DES_ENCRYPT;
	else if(strcmp(argv[4], "-dec")==0)
		enc = DES_DECRYPT;
	else {
		printf("Blad skladni!\n");
		return 1;
	}
	
	if(strcmp(argv[5], "-ecb")==0)
		mode = ECB;
	else if(strcmp(argv[5], "-cbc")==0)
		mode = CBC;
	else {
		printf("Blad skladni!\n");
		return 1;
	}
	
	DES_key_schedule sch;
	DES_set_key_checked(&key, &sch);
	
	unsigned char in[9], out[9], ivec[8] = "\0\0\0\0\0\0\0\0";
	in[8]=out[8]=0;
	int brd;
	
	if(mode == ECB) {
		while( (brd=fread(in, sizeof(unsigned char), 8, fin)) > 0) {
			if(enc == DES_ENCRYPT)
				in[brd]='\0';
			DES_ecb_encrypt((DES_cblock*)in, (DES_cblock*)out, &sch, enc);
			fwrite(out, sizeof(unsigned char), (enc == DES_DECRYPT ? strlen(out) : 8), fout);
			if(feof(fin))
				break;
		}
	}
	else if(mode == CBC) {
		while( (brd=fread(in, sizeof(unsigned char), 8, fin)) > 0) {
			if(enc == DES_ENCRYPT)
				in[brd]=0;
			DES_ncbc_encrypt(in, out, 8, &sch, (DES_cblock*)ivec, enc);
			fwrite(out, 1, (enc == DES_DECRYPT ? strlen(out) : 8), fout);
			if(feof(fin))
				break;
		}
	}
	
	fclose(fin);	
	fclose(fout);
	
	return 0;
}

