#include <linux/kernel.h>
#include "iv.h"

void generate_iv(sector_t sector, const u8 *key, u8 *iv) {
    memset(iv, 0, 16);

    // Optimized logic for generating IV
    // Using only part of the key and sector number to generate IV
    memcpy(iv, &sector, sizeof(sector_t));
    memcpy(iv + sizeof(sector_t), key, 8); // Use first 8 bytes of the key for IV

    printk(KERN_INFO "USB Encryption Layer: Optimized IV generation for sector %llu\n", sector);
}
