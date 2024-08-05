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
    sector_t sector = 0;
    u8 derived_key[32];
    u8 iv[16];

    // Assuming password is provided by the user for every ioctl call
    if (copy_from_user(password, (char __user *)arg, 64)) {
        return -EFAULT; // Handle error if unable to retrieve password
    }

    // Derive key from the password
    if (derive_key_from_password(password, strlen(password), NULL, derived_key, 32) != 0) {
        printk(KERN_ERR "USB Encryption Layer: Invalid password or key derivation failed\n");
        return -EINVAL;
    }

    switch (cmd) {
        case IOCTL_READ:
            generate_iv(sector, derived_key, iv);
            aes_decrypt(buffer, buffer, derived_key, iv); // Decrypt data
            if (usb_bio_read(NULL, sector, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_READ successful\n");
            }
            break;
        case IOCTL_WRITE:
            generate_iv(sector, derived_key, iv);
            aes_encrypt(buffer, buffer, derived_key, iv); // Encrypt data
            if (usb_bio_write(NULL, sector, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_WRITE successful\n");
            }
            break;
        default:
            return -EINVAL;
    }
    return 0;
}
