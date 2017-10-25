#include "../kyber.h"
#include "../poly.h"
#include "../randombytes.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NTESTS 100

int test_keys()
{
  unsigned char key_a[KYBER_SHAREDKEYBYTES], key_b[KYBER_SHAREDKEYBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_BYTES];
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  int i;

  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    crypto_kem_keypair_pm(pk, sk_a);

    //Bob derives a secret key and creates a response
    crypto_kem_enc_pm(sendb, key_b, pk);
  
    //Alice uses Bobs response to get her secret key
    crypto_kem_dec_pm(key_a, sendb, sk_a);

    if(memcmp(key_a, key_b, KYBER_SHAREDKEYBYTES))
      printf("i=%d, ERROR keys\n",i);
  }

  return 0;
}


int test_invalid_sk_a()
{
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  unsigned char key_a[KYBER_SHAREDKEYBYTES], key_b[KYBER_SHAREDKEYBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_BYTES];
  int i;

  FILE *urandom = fopen("/dev/urandom", "r");
  
  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    crypto_kem_keypair_pm(pk, sk_a);

    //Bob derives a secret key and creates a response
    crypto_kem_enc_pm(sendb, key_b, pk);

    //Replace secret key with random values
    fread(sk_a, KYBER_SECRETKEYBYTES, 1, urandom); 
  
    //Alice uses Bobs response to get her secre key
    crypto_kem_dec_pm(key_a, sendb, sk_a);
    
    if(!memcmp(key_a, key_b, KYBER_SHAREDKEYBYTES))
      printf("ERROR invalid sk_a\n");
  }

  fclose(urandom);

  return 0;
}


int test_invalid_ciphertext()
{
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  unsigned char key_a[KYBER_SHAREDKEYBYTES], key_b[KYBER_SHAREDKEYBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_BYTES];
  int i;
  size_t pos;

  FILE *urandom = fopen("/dev/urandom", "r");
  
  for(i=0; i<NTESTS; i++)
  {
    fread(&pos, sizeof(int), 1, urandom);

    //Alice generates a public key
    crypto_kem_keypair_pm(pk, sk_a);

    //Bob derives a secret key and creates a response
    crypto_kem_enc_pm(sendb, key_b, pk);

    //Change some byte in the ciphertext (i.e., encapsulated key)
    sendb[pos % KYBER_BYTES] ^= 23;
  
    //Alice uses Bobs response to get her secre key
    crypto_kem_dec_pm(key_a, sendb, sk_a);

    if(!memcmp(key_a, key_b, KYBER_SHAREDKEYBYTES))
      printf("ERROR invalid ciphertext\n");
  }

  fclose(urandom);

  return 0;
}

int main(){

  test_keys();
  test_invalid_sk_a();
  test_invalid_ciphertext();
  
  printf("KYBER_SECRETKEYBYTES: %d\n",KYBER_SECRETKEYBYTES);
  printf("KYBER_PUBLICKEYBYTES: %d\n",KYBER_PUBLICKEYBYTES);
  printf("KYBER_BYTES:          %d\n",KYBER_BYTES);

  return 0;
}
