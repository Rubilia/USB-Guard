#include <crypto/pbkdf2.h>
#include <crypto/hash.h>
#include <linux/crypto.h>
#include <linux/kernel.h>
#include "key_derivation.h"

int derive_key_from_password(const char *password, size_t password_len, u8 *salt, u8 *derived_key, size_t key_len) {
    struct crypto_shash *tfm = crypto_alloc_shash("pbkdf2-hmac-sha256", 0, 0);
    if (IS_ERR(tfm)) {
        printk(KERN_ERR "USB Encryption Layer: Error allocating PBKDF2\n");
        return PTR_ERR(tfm);
    }

    int ret = crypto_shash_setkey(tfm, password, password_len);
    if (ret) {
        printk(KERN_ERR "USB Encryption Layer: Error setting PBKDF2 key\n");
        return ret;
    }

    ret = pbkdf2(tfm, password, password_len, salt, 16, 10000, derived_key, key_len);
    if (ret) {
        printk(KERN_ERR "USB Encryption Layer: PBKDF2 key derivation failed\n");
    }
    
    crypto_free_shash(tfm);
    return ret;
}
