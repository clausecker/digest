#include <string.h>
#include "finalize.h"
#include "sha1.h"

enum {
	SHA1_K0 = 0x5A827999,
	SHA1_K1 = 0x6ED9EBA1,
	SHA1_K2 = 0x8F1BBCDC,
	SHA1_K3 = 0xCA62C1D6,
};

const struct sha1_state sha1_init =
	{ 0, { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 } };

/* assumes that p points to exactly 64 bytes of data. */
extern void sha1_block(union digest_state *u, const uint8_t *p) {
	struct sha1_state *s = (struct sha1_state *)u;
	uint32_t a = s->h[0], b = s->h[1], c = s->h[2], d = s->h[3], e = s->h[4];
	uint32_t w[16], f, a5, b30, t, tmp, j;
	int i;

	s->n++;

	for (i = 0; i < 16; i++) {
		j = i * 4;
		w[i] = p[j] << 24 | p[j+1] << 16 | p[j+2] << 8 | p[j+3];
	}

	i = 0;

	for (; i < 16; i++) {
		f = b&c | ~b&d;
		a5 = a<<5 | a>>(32-5);
		b30 = b<<30 | b>>(32-30);
		t = a5 + f + e + w[i&0xf] + SHA1_K0;
		e = d; d = c; c = b30; b = a; a = t;
	}

	for (; i < 20; i++) {
		tmp = w[(i-3)&0xf] ^ w[(i-8)&0xf] ^ w[(i-14)&0xf] ^ w[i&0xf];
		w[i&0xf] = tmp<<1 | tmp>>(32-1);

		f = b&c | ~b&d;
		a5 = a<<5 | a>>(32-5);
		b30 = b<<30 | b>>(32-30);
		t = a5 + f + e + w[i&0xf] + SHA1_K0;
		e = d; d = c; c = b30; b = a; a = t;
	}

	for (; i < 40; i++) {
		tmp = w[(i-3)&0xf] ^ w[(i-8)&0xf] ^ w[(i-14)&0xf] ^ w[i&0xf];
		w[i&0xf] = tmp<<1 | tmp>>(32-1);

		f = b ^ c ^ d;
		a5 = a<<5 | a>>(32-5);
		b30 = b<<30 | b>>(32-30);
		t = a5 + f + e + w[i&0xf] + SHA1_K1;
		e = d; d = c; c = b30; b = a; a = t;
	}

	for (; i < 60; i++) {
		tmp = w[(i-3)&0xf] ^ w[(i-8)&0xf] ^ w[(i-14)&0xf] ^ w[i&0xf];
		w[i&0xf] = tmp<<1 | tmp>>(32-1);

		f = (b|c)&d | b&c;
		a5 = a<<5 | a>>(32-5);
		b30 = b<<30 | b>>(32-30);
		t = a5 + f + e + w[i&0xf] + SHA1_K2;
		e = d; d = c; c = b30; b = a; a = t;
	}

	for (; i < 80; i++) {
		tmp = w[(i-3)&0xf] ^ w[(i-8)&0xf] ^ w[(i-14)&0xf] ^ w[i&0xf];
		w[i&0xf] = tmp<<1 | tmp>>(32-1);

		f = b ^ c ^ d;
		a5 = a<<5 | a>>(32-5);
		b30 = b<<30 | b>>(32-30);
		t = a5 + f + e + w[i&0xf] + SHA1_K3;
		e = d; d = c; c = b30; b = a; a = t;
	}

	s->h[0] += a; s->h[1] += b; s->h[2] += c; s->h[3] += d; s->h[4] += e;
}

/* expects that n < 64. Overwrites p. */
extern void sha1_final(union digest_state *u, uint8_t *p, size_t n, uint8_t *d) {
	finalize(u, p, n, d, SHA1_DIGESTLEN, LITTLE, sha1_block);
}
