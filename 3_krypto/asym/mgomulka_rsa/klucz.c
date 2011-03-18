#include <openssl/rsa.h>
 #include <openssl/engine.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  RSA * param = NULL;
  FILE * priv_key;
  FILE * pub_key;

  param = RSA_generate_key(1024, 65537, NULL, NULL);
  if(param == NULL) {
    printf("Blad przy generowaniu kluczy\n");
    return 1;
  }
  else {
    priv_key = fopen("priv.key", "wb");
    pub_key = fopen("pub.key", "wb");

    unsigned char* buf = NULL;
    int len = i2d_RSAPublicKey(param, NULL);
    buf = (unsigned char*) malloc(len*sizeof(unsigned char));
    unsigned char* p = buf;
    i2d_RSAPublicKey(param, &p);
    fwrite(buf, sizeof(unsigned char), len, pub_key);
    free(buf);

    buf = NULL;
    len = i2d_RSAPrivateKey(param, NULL);
    buf = (unsigned char*) calloc(len, sizeof(unsigned char));
    p = buf;
    i2d_RSAPrivateKey(param, &p);
    fwrite(buf, sizeof(unsigned char), len, priv_key);
    free(buf);

    fclose(priv_key);
    fclose(pub_key);
  }

  return 0;
}
