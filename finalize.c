/* Copyright (c) 2013, Robert Clausecker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE. */

#include <string.h>
#include "finalize.h"

extern void finalize(union digest_state *u, uint8_t *p, size_t n, uint8_t *d,
	size_t dlen, int endian, digest_block *block) {

	uint32_t *sn = (uint32_t*)u, len = *sn*64 + n, hi, *h = sn + 1;
	unsigned i;

	/* padding */
	p[n++] = 0x80;
	if (n <= 56) memset(p + n, 0, 64 - n - 5);
	else {
		memset(p + n, 0, 64 - n);
		block(u, p);
		memset(p, 0, 56);
	}

	hi = len >> 29; len <<= 3;
	if (endian == LITTLE) {
		p[56] = p[57] = p[58] = 0;
		p[59] = hi;
		p[60] = len >> 24;
		p[61] = len >> 16;
		p[62] = len >>  8;
		p[63] = len >>  0;
	} else {
		p[56] = len;
		p[57] = len >> 8;
		p[58] = len >> 16;
		p[59] = len >> 24;
		p[60] = hi;
		p[61] = p[62] = p[63] = 0;
	}

	block(u, p);

	for (i = 0; i < dlen>>2; i++) {
		if (endian == LITTLE) {
			d[i*4]   = h[i] >> 24;
			d[i*4+1] = h[i] >> 16;
			d[i*4+2] = h[i] >>  8;
			d[i*4+3] = h[i] >>  0;
		} else {
			d[i*4]   = h[i] >>  0;
			d[i*4+1] = h[i] >>  8;
			d[i*4+2] = h[i] >> 16;
			d[i*4+3] = h[i] >> 24;
		}
	}
}
