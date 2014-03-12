#include <string.h>
#include "finalize.h"
#include "sha2.h"

const struct sha256_state sha256_init = { 0, {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
}};

const struct sha224_state sha224_init = { 0, {
	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
	0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4,
}};

/* SHA2 round constants */
static const uint32_t sha2const[64] =
#include "sha2tab.h"

#define ROR(n, m) ((n) >> (m) | (n) << (32 - (m)))

extern void sha256_block(union digest_state *u, const uint8_t *p) {
	struct sha256_state *s = (struct sha256_state *)u;
	uint32_t w[64];
	int i;
	uint32_t a = s->h[0], b = s->h[1], c = s->h[2], d = s->h[3];
	uint32_t e = s->h[4], f = s->h[5], g = s->h[6], h = s->h[7];
	uint32_t v1, t1, v2, t2;

	s->n++;

	for (i = 0; i < 16; i++) w[i] = p[4*i]<<24 | p[4*i+1]<<16 | p[4*i+2]<<8 | p[4*i+3];

	for (; i < 64; i++) {
		v1 = w[i-2];
		t1 = ROR(v1, 17) ^ ROR(v1, 19) ^ (v1 >> 10);
		v2 = w[i-15];
		t2 = ROR(v2, 7) ^ ROR(v2, 18) ^ (v2 >> 3);
		w[i] = t1 + w[i-7] + t2 + w[i-16];
	}

	for (i = 0; i < 64; i++) {
		t1 = h + (ROR(e, 6) ^ ROR(e, 11) ^ ROR(e, 25)) + (e&f ^ ~e&g) + sha2const[i] + w[i];
		t2 = (ROR(a, 2) ^ ROR(a, 13) ^ ROR(a, 22)) + (a&b ^ a&c ^ b&c);

		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	s->h[0] += a;
	s->h[1] += b;
	s->h[2] += c;
	s->h[3] += d;
	s->h[4] += e;
	s->h[5] += f;
	s->h[6] += g;
	s->h[7] += h;
}

/* expects that n < 64. Overwrites p. */
extern void sha256_final(union digest_state *u, uint8_t *p, size_t n, uint8_t *d) {
	finalize(u, p, n, d, SHA256_DIGESTLEN, LITTLE, sha256_block);
}

extern void sha224_final(union digest_state *u, uint8_t *p, size_t n, uint8_t *d) {
	finalize(u, p, n, d, SHA224_DIGESTLEN, LITTLE, sha224_block);
}
