#ifndef SHA1_H
#define SHA1_H

#include "digest.h"

enum {
	SHA1_BLOCKLEN = 64,
	SHA1_DIGESTLEN = 20,
};

extern const struct sha1_state {
	uint32_t n;
	uint32_t h[5];
} sha1_init;

extern digest_block sha1_block;
extern digest_final sha1_final;

#endif /* SHA1_H */
