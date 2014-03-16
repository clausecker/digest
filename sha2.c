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
static const uint32_t sha2const[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

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
