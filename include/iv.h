#ifndef __IV_H__
#define __IV_H__

#include <linux/types.h>

void generate_iv(sector_t sector, const u8 *key, u8 *iv);

#endif
