#include "sysv.h"

const struct sysv_state sysv_init;

extern void sysv_block(union digest_state *u, const uint8_t *p) {
	uint32_t h = *(uint32_t*)u;
	int i;

	for (i = 0; i < SYSV_BLOCKLEN; i++) h += p[i];
	*(uint32_t*)u = h;
}

extern void sysv_final(union digest_state *u, uint8_t *p, size_t len, uint8_t *d) {
	uint32_t h = *(uint32_t*)u;
	size_t i;

	for (i = 0; i < len; i++) h += p[i];
	h = (h & 0xffff) + (h >> 16);
	h = (h & 0xffff) + (h >> 16);

	d[0] = h >> 8;
	d[1] = h >> 0;
}
