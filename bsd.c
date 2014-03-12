#include "bsd.h"

const struct bsd_state bsd_init;

static uint16_t consume(uint16_t sum, const uint8_t *p, size_t len) {
	size_t i;

	for (i = 0; i < len; i++) {
		sum = (sum >> 1) + (sum << 15);
		sum += p[i];
	}

	return sum;
}

extern void bsd_block(union digest_state *u, const uint8_t *p) {
	*(uint16_t*)u = consume(*(uint16_t*)u, p, BSD_BLOCKLEN);
}

extern void bsd_final(union digest_state *u, uint8_t *p, size_t len, uint8_t *d) {
	uint32_t h = consume(*(uint16_t*)u, p, len);

	/* write result in big endian */
	d[0] = h >> 8;
	d[1] = h >> 0;
}
