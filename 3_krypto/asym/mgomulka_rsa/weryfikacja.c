#include <openssl/rsa.h>
#include <openssl/md5.h>
 #include <openssl/engine.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  RSA* param = NULL;
  FILE* pub_key;
  FILE* input;
  FILE* signature;
  long size_klucz, size_podpis;
  unsigned char data[1024];
  unsigned char md5_sum[MD5_DIGEST_LENGTH];
  int bytes;

  if(argc != 3) {
    printf("Niewlasciwa liczba argumentow. Skladnia: %s <plik_z_podpisem> <plik_podpisany>\n");
    return 1;
  }

  signature = fopen(argv[1], "rb");
  input = fopen(argv[2], "rb");
  pub_key = fopen("pub.key", "rb");
  if(signature == NULL || input == NULL || pub_key == NULL) {
    printf("Blad podczas otwierania plikow\n");
    return 1;
  }

  MD5_CTX ctx;
  if(!MD5_Init(&ctx)) {
    printf("Blad funkcji MD5_Init\n");
    return 1;
  }
  while(bytes = fread(data, sizeof(unsigned char), 1024, input)) {
    if(!MD5_Update(&ctx, data, bytes)) {
      printf("Blad funkcji MD5_Update\n");
      return 1;
    }
  }
  if(!MD5_Final(md5_sum, &ctx)) {
    printf("Blad funkcji MD5_Update\n");
    return 1;
  }

  fseek(pub_key, 0, SEEK_END);
  size_klucz=ftell(pub_key);
  rewind(pub_key);

  fseek(signature, 0, SEEK_END);
  size_podpis=ftell(signature);
  rewind(signature);

  unsigned char* buf_klucz = (unsigned char*) malloc(size_klucz*sizeof(unsigned char));
  fread(buf_klucz, sizeof(unsigned char*), size_klucz, pub_key);

  unsigned char* buf_podpis = (unsigned char*) malloc(size_podpis*sizeof(unsigned char));
  fread(buf_podpis, sizeof(unsigned char*), size_podpis, signature);

  const unsigned char* p = buf_klucz;
  d2i_RSAPublicKey(&param, &p, size_klucz);
  if(param == NULL) {
    printf("Blad podczas tworzenia struktury dsa\n");
    return 1;
  }

  int w;
  w=RSA_verify(NID_md5, md5_sum, sizeof(md5_sum), buf_podpis, size_podpis, param);
  if(w==1)
    printf("Podpis jest prawidlowy\n");
  else
    printf("Podpis jest nieprawidlowy.\n");

  free(buf_klucz);
  free(buf_podpis);
  fclose(signature);
  fclose(input);
  fclose(pub_key);

  return 0;
}
