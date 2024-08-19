#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "ioctl_defines.h"
#include "bio_operations.h"
#include "key_derivation.h"
#include "iv.h"
#include "encryption.h"

// Added final review comments and cleaned up code for ioctl handling
static long usb_encryption_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    char buffer[512];
    char password[64];
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
            memset(buffer, 0, sizeof(buffer));
            generate_iv(0, derived_key, iv);
            aes_decrypt(buffer, buffer, derived_key, iv);
            if (usb_bio_read(NULL, 0, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_READ successful\n");
            }
            break;
        case IOCTL_WRITE:
            memset(buffer, 0, sizeof(buffer));
            generate_iv(0, derived_key, iv);
            aes_encrypt(buffer, buffer, derived_key, iv);
            if (usb_bio_write(NULL, 0, buffer) == 0) {
                printk(KERN_INFO "USB Encryption Layer: IOCTL_WRITE successful\n");
            }
            break;
        default:
            return -EINVAL;
    }

    return 0;
}
