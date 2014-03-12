#ifndef BSD_H
#define BSD_H

#include <stdio.h>
#include "digest.h"

enum {
	/* bsdsum processes input in one byte steps. Let's try to synchronize
	 * the summing with the read buffer */
	BSD_BLOCKLEN = BUFSIZ,
	BSD_DIGESTLEN = sizeof(uint16_t),
};

extern const struct bsd_state {
	uint16_t h;
} bsd_init;

extern digest_block bsd_block;
extern digest_final bsd_final;

#endif /* BSD_H */
