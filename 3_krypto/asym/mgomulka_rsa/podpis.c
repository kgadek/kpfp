#include <openssl/rsa.h>
#include <openssl/md5.h>
 #include <openssl/engine.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Niewlasciwa liczba argumentow. Skladnia: %s <plik_do_podpisu>\n", argv[0]);
    return 1;
  }

  RSA* param = NULL;
  unsigned char data[1024];
  unsigned char md5_sum[MD5_DIGEST_LENGTH];
  FILE* priv_key;
  FILE* input;
  FILE* output;
  long size;
  int bytes;

  priv_key = fopen("priv.key", "rb");
  input = fopen(argv[1], "rb");
  if(priv_key == NULL || input == NULL) {
    printf("Blad podczas otwierania plikow");
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

  fseek(priv_key, 0, SEEK_END);
  size = ftell(priv_key);
  rewind(priv_key);

  unsigned char* buf = (unsigned char*) malloc(size*sizeof(unsigned char));
  fread(buf, sizeof(unsigned char*), size, priv_key);

  const unsigned char* p = buf;
  d2i_RSAPrivateKey(&param, &p, size);
  if(param == NULL) {
    printf("Blad podczas tworzenia struktury rsa\n");
    return 1;
  }

  unsigned char* sigret = (unsigned char*) malloc(RSA_size(param));
  unsigned int siglen;

  if(!RSA_sign(NID_md5, md5_sum, sizeof(md5_sum), sigret, &siglen, param)) {
    printf("Blad podczas podpisywania pliku\n");
    return 1;
  }

  output = fopen("signature.sig", "wb");
  fwrite(sigret, sizeof(unsigned char), siglen, output);

  free(buf);
  free(sigret);
  fclose(output);
  fclose(input);
  fclose(priv_key);

  return 0;
}
