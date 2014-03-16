#ifndef CKSUM_H
#define CKSUM_H

#include <stdio.h>
#include "digest.h"

enum {
	/* cksum processes input in one byte steps. Let's try to synchronize the
	 * summing with the read buffer */
	CKSUM_BLOCKLEN = BUFSIZ,
	CKSUM_DIGESTLEN = sizeof(uint32_t),
};

extern const struct cksum_state {
	uint32_t n;
	uint32_t h;
} cksum_init;

extern digest_block cksum_block;
extern digest_final cksum_final;

#endif /* CKSUM_H */
