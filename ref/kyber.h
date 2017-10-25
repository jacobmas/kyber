#ifndef KYBER_H
#define KYBER_H

void crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

void crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);

void crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);

void crypto_kem_keypair_pm(unsigned char *pk, unsigned char *sk);

void crypto_kem_enc_pm(unsigned char *c, unsigned char *k, const unsigned char *pk);

void crypto_kem_dec_pm(unsigned char *k, const unsigned char *c, const unsigned char *sk);

#endif
