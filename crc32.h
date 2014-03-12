#ifndef CRC32_H
#define CRC32_H

#include <stdio.h>
#include "digest.h"

enum {
	/* CRC32 processes input in one byte steps. Let's try to synchronize the
	 * summing with the read buffer */
	CRC32_BLOCKLEN = BUFSIZ,
	CRC32_DIGESTLEN = sizeof(uint32_t),
};

extern const struct crc32_state {
	uint32_t h;
} crc32_init;

extern digest_block crc32_block;
extern digest_final crc32_final;

#endif /* CRC32_H */
