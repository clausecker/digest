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
