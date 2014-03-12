#ifndef MD5_H
#define MD5_H

#include "digest.h"

enum {
	MD5_BLOCKLEN = 64,
	MD5_DIGESTLEN = 16,
};

extern const struct md5_state {
	uint32_t n;
	uint32_t h[4];
} md5_init;

extern digest_block md5_block;
extern digest_final md5_final;

#endif /* SHA1_H */
