#ifndef ADLER32_H
#define ADLER32_H

#include <stdio.h>
#include "digest.h"

enum {
	ADLER32_BLOCKLEN = BUFSIZ,
	ADLER32_DIGESTLEN = 2*sizeof(uint16_t),
};

extern const struct adler32_state {
	uint16_t a;
	uint16_t b;
} adler32_init;

extern digest_block adler32_block;
extern digest_final adler32_final;

#endif /* SHA1_H */
