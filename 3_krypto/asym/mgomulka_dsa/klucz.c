#include <openssl/dsa.h>
#include <openssl/engine.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  DSA * param = NULL;
  FILE * priv_key;
  FILE * pub_key;
  param = DSA_generate_parameters(1024, NULL, 0, NULL, NULL, NULL, NULL);
  if (param == NULL) {
      printf("Blad przy generowaniu parametrow\n");
      return 1;
  }
  else {
    if(DSA_generate_key(param) != 1) {
      printf("Blad przy generowaniu kluczy\n");
      return 1;
    }
    else {
      priv_key = fopen("priv.key", "wb");
      pub_key = fopen("pub.key", "wb");

      unsigned char* buf = NULL;
      int len = i2d_DSAPublicKey(param, NULL);
      buf = (unsigned char*) malloc(len*sizeof(unsigned char));
      unsigned char* p = buf;
      i2d_DSAPublicKey(param, &p);
      fwrite(buf, sizeof(unsigned char), len, pub_key);
      free(buf);

      buf = NULL;
      len = i2d_DSAPrivateKey(param, NULL);
      buf = (unsigned char*) calloc(len, sizeof(unsigned char));
      p = buf;
      i2d_DSAPrivateKey(param, &p);
      fwrite(buf, sizeof(unsigned char), len, priv_key);
      free(buf);

      fclose(priv_key);
      fclose(pub_key);
      }
  }

  return 0;
}
