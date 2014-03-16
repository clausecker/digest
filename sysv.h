#ifndef SYSV_H
#define SYSV_H

#include <stdio.h>
#include "digest.h"

enum {
	SYSV_BLOCKLEN = BUFSIZ,
	SYSV_DIGESTLEN = sizeof(uint16_t),
};

extern const struct sysv_state {
	uint32_t h;
} sysv_init;

extern digest_block sysv_block;
extern digest_final sysv_final;

#endif /* SYSV_H */
