#ifndef SHA2_H
#define SHA2_H

#include "digest.h"

/* the SHA256 and SHA224 algorithms. These two share a lot of code. */
enum {
	SHA256_BLOCKLEN = 64,
	SHA256_DIGESTLEN = 32,
	SHA224_BLOCKLEN = 64,
	SHA224_DIGESTLEN = 28,
};

extern const struct sha256_state {
	uint32_t n;
	uint32_t h[8];
} sha256_init, sha224_init;

#define sha224_state sha256_state

extern digest_block sha256_block;
extern digest_final sha256_final;

/* SHA224 has the same block function as SHA256 */
#define sha224_block sha256_block
extern digest_final sha224_final;

#endif /* SHA2_H */
