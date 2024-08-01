#ifndef __ENCRYPTION_H__
#define __ENCRYPTION_H__

int aes_encrypt(const u8 *plaintext, u8 *ciphertext, const u8 *key, const u8 *iv);
int aes_decrypt(const u8 *ciphertext, u8 *plaintext, const u8 *key, const u8 *iv);

#endif
