#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

int main(int argc, char *argv[]) {
	FILE *in, *out;
	char password[256];
	unsigned char md5_sum[MD5_DIGEST_LENGTH];
	unsigned char buffer_in[MD5_DIGEST_LENGTH];
	unsigned char buffer_out[MD5_DIGEST_LENGTH];
	int i, bytes;
	
	if (argc < 3) {
		printf("Prawidlowe wywolanie: xor plik_we plik_wy\n");
		return 1;
	}
	
	if ((in = fopen(argv[1], "r")) == NULL || (out = fopen(argv[2], "w")) == NULL) {
		printf("Nie moge otworzyc plikow\n");
		return 1;
	}
	
	printf("Prosze podac haslo:\n");
	scanf("%s", &password);
	
	MD5(password, strlen(password), md5_sum);
	
	/*
	printf("MD5 sum: ");
	
	for (i = 0; i < MD5_DIGEST_LENGTH; i++)
		printf("%02x", md5_sum[i]);
	printf("\n");
	*/
	while (!feof(in)) {
		if ((bytes = fread(buffer_in, sizeof(char), MD5_DIGEST_LENGTH, in)) == -1) {
			printf("Blad podczas czytania pliku\n");
			return 1;
		}
		
		for (i = 0; i < bytes; i++)
			buffer_out[i] = buffer_in[i] ^ md5_sum[i];
	
		if (fwrite(buffer_out, sizeof(char), bytes, out) != bytes) {
			printf("Blad podczas pisania do pliku\n");
			return 1;
		}
	}
	
	fclose(in);
	fclose(out);
	return 0;
}
