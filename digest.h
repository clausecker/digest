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

#ifndef DIGEST_H
#define DIGEST_H

#include <stdint.h>
#include <stddef.h>

/* a union digest_state is a union of all available digest state types. */
union digest_state;

typedef void digest_block(union digest_state*,const uint8_t*);
typedef void digest_final(union digest_state*,uint8_t*,size_t,uint8_t*);

struct digest_descr {
	const union digest_state *init;
	digest_block *block;
	digest_final *final;
	const char name[9];
	const char fchar;	/* formatting character */
	const char hide;	/* hide digest from help screen? */
	unsigned short int blocklen;
	unsigned short int digestlen;
	unsigned short int statelen;
};

#define DIGEST_DESCR(hide, format, str, NAME, name) {	\
	(const union digest_state*)&name ## _init,	\
	name ## _block,					\
	name ## _final,					\
	str "\0",					\
	format,						\
	hide,						\
	NAME ## _BLOCKLEN,				\
	NAME ## _DIGESTLEN,				\
	sizeof(struct name ## _state),			\
}

#endif /* DIGEST_H */
