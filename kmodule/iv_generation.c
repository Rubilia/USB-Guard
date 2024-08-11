#include <linux/kernel.h>
#include "iv.h"

void generate_iv(sector_t sector, const u8 *key, u8 *iv) {
    memset(iv, 0, 16);
    memcpy(iv, &sector, sizeof(sector_t));
    
    // Debug log to track IV generation process
    printk(KERN_INFO "USB Encryption Layer: Generating IV for sector %llu\n", sector);
    
    // More logic for using key in IV generation can be added here
}
