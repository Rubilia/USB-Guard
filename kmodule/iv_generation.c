#include <linux/kernel.h>
#include "iv.h"

void generate_iv(sector_t sector, const u8 *key, u8 *iv) {
    // Use the block index (sector) and key to generate a unique IV for each block
    memset(iv, 0, 16);
    memcpy(iv, &sector, sizeof(sector_t)); // Use sector as part of IV
    // Other logic for IV generation (truncated for simplicity)
    printk(KERN_INFO "USB Encryption Layer: IV generated for sector %llu\n", sector);
}
