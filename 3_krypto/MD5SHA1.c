/* made by Sandra Imiela & jerzy Muller (partially ;P) */

#include <stdio.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

const int size = 1024;

void md5(FILE * f)
{
 MD5_CTX ctx;
 unsigned char tab[MD5_DIGEST_LENGTH];
 MD5_Init(&ctx);
 unsigned char bufor[size];
 int read;
 
 while (!feof(f))
       {
        read = fread(bufor, 1, size, f);
        MD5_Update(&ctx, bufor, read);
       }
 
 MD5_Final(tab, &ctx);
 
 int i;
 for (i = 0; i < MD5_DIGEST_LENGTH; i++)
     printf("%02x", tab[i]);
 printf("\n");   
}

void sha1(FILE * f)
{
 SHA_CTX ctx;
 unsigned char tab[SHA_DIGEST_LENGTH];
 SHA1_Init(&ctx);
 unsigned char bufor[size];
 int read;
 
 while (!feof(f))
       {
        read = fread(bufor, 1, size, f);
        SHA1_Update(&ctx, bufor, read);
       }
 
 SHA1_Final(tab, &ctx);
 
 int i;
 for (i = 0; i < SHA_DIGEST_LENGTH; i++)
     printf("%02x", tab[i]);
 printf("\n");   
}

int main(int argc, char* argv[])
{
 if (argc != 3)
    {
     printf("Nieprawidlowe argumenty: \ntest sha1|md5 file\n");
     return 1;
    }
        
 FILE* f = fopen(argv[2], "r");
 
 if (!f)
    {
     printf("Nie udalo sie otworzyc pliku\n");
     return 1;
    }
        
 if (strcmp (argv[1], "sha1") == 0)
    sha1(f);
 else
     if (strcmp (argv[1], "md5") == 0)
        md5(f);
     else
         {
          printf("Nieprawidlowe argumenty: \ntest sha1|md5 file\n");
          return 1;
         }
 
 fclose(f);
 return 0;
}
