#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "ioctl_defines.h"
#include "bio_operations.h"
#include "key_derivation.h"
#include "iv.h"
#include "encryption.h"

static long usb_encryption_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    char buffer[512];
    sector_t sector = 0;
    u8 derived_key[32];
    u8 iv[16];

    switch (cmd) {
        case IOCTL_READ:
            generate_iv(sector, derived_key, iv);  // Generate IV for block
            aes_decrypt(buffer, buffer, derived_key, iv); // Decrypt
            if (usb_bio_read(NULL, sector, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_READ successful\n");
            }
            break;
        case IOCTL_WRITE:
            generate_iv(sector, derived_key, iv);  // Generate IV for block
            aes_encrypt(buffer, buffer, derived_key, iv); // Encrypt
            if (usb_bio_write(NULL, sector, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_WRITE successful\n");
            }
            break;
        default:
            return -EINVAL;
    }
    return 0;
}
