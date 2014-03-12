#include "adler32.h"

const struct adler32_state adler32_init;

static void consume(struct adler32_state *s, const uint8_t *p, size_t len) {
	size_t i;
	uint32_t a = s->a, b = s->b;

	for (i = 0; i < len; i++) {
		a += p[i];
		if (a >= 65521) a -= 65521;
		b += a;
		if (b >= 65521) b -= 65521;
	}

	s->a = a;
	s->b = b;
}

extern void adler32_block(union digest_state *u, const uint8_t *p) {
	struct adler32_state *s = (struct adler32_state*)u;

	consume(s, p, ADLER32_BLOCKLEN);
}

extern void adler32_final(union digest_state *u, uint8_t *p, size_t len, uint8_t *d) {
	struct adler32_state *s = (struct adler32_state*)u;

	consume(s, p, len);

	d[0] = s->b >> 8;
	d[1] = s->b >> 0;
	d[2] = s->a >> 8;
	d[3] = s->a >> 0;
}

