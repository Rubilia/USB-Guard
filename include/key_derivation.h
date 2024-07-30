#ifndef __KEY_DERIVATION_H__
#define __KEY_DERIVATION_H__

#include <linux/types.h>

int derive_key_from_password(const char *password, size_t password_len, u8 *salt, u8 *derived_key, size_t key_len);

#endif
