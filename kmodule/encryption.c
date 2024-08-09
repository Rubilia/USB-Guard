#include <crypto/skcipher.h>
#include <linux/kernel.h>
#include <linux/crypto.h>
#include "encryption.h"

static struct crypto_skcipher *tfm;
static struct skcipher_request *req;

int aes_encrypt(const u8 *plaintext, u8 *ciphertext, const u8 *key, const u8 *iv) {
    struct scatterlist sg;
    struct skcipher_request *req = skcipher_request_alloc(tfm, GFP_KERNEL);
    if (!req) {
        printk(KERN_ERR "USB Encryption Layer: Could not allocate skcipher request\n");
        return -ENOMEM;
    }

    skcipher_request_set_crypt(req, &sg, &sg, 512, iv);
    if (crypto_skcipher_encrypt(req)) {
        printk(KERN_ERR "USB Encryption Layer: AES encryption failed\n");
        return -EFAULT;
    }

    skcipher_request_free(req);
    return 0;
}

int aes_decrypt(const u8 *ciphertext, u8 *plaintext, const u8 *key, const u8 *iv) {
    struct scatterlist sg;
    struct skcipher_request *req = skcipher_request_alloc(tfm, GFP_KERNEL);
    if (!req) {
        printk(KERN_ERR "USB Encryption Layer: Could not allocate skcipher request\n");
        return -ENOMEM;
    }

    skcipher_request_set_crypt(req, &sg, &sg, 512, iv);
    if (crypto_skcipher_decrypt(req)) {
        printk(KERN_ERR "USB Encryption Layer: AES decryption failed\n");
        return -EFAULT;
    }

    skcipher_request_free(req);
    return 0;
}
