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
    char password[64];
    unsigned int partial_size;
    sector_t sector = 0;
    u8 derived_key[32];
    u8 iv[16];

    if (copy_from_user(password, (char __user *)arg, 64)) {
        return -EFAULT; 
    }

    if (derive_key_from_password(password, strlen(password), NULL, derived_key, 32) != 0) {
        printk(KERN_ERR "USB Encryption Layer: Invalid password\n");
        return -EINVAL;
    }

    switch (cmd) {
        case IOCTL_READ:
            generate_iv(sector, derived_key, iv);
            aes_decrypt(buffer, buffer, derived_key, iv);
            if (usb_bio_read(NULL, sector, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_READ successful\n");
            }
            break;
        case IOCTL_WRITE:
            // Handle partial block write
            if (copy_from_user(&partial_size, (unsigned int __user *)arg + 64, sizeof(unsigned int))) {
                return -EFAULT;
            }
            if (partial_size < 512) {
                memset(buffer + partial_size, 0, 512 - partial_size); // Zero-fill remaining block
            }
            generate_iv(sector, derived_key, iv);
            aes_encrypt(buffer, buffer, derived_key, iv);
            if (usb_bio_write(NULL, sector, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_WRITE successful (partial block)\n");
            }
            break;
        default:
            return -EINVAL;
    }
    return 0;
}
