#ifndef FINALIZE_H
#define FINALIZE_H

#include "digest.h"

enum {
	LITTLE	= 0,
	BIG	= 1,
};

/* generic finalization function for digests with 64 bytes block length that
 * use a padding scheme similar to that of MD5, SHA1 and SHA2. Requires that the
 * first field of union digest_descr is the length field. */
extern void finalize(union digest_state*,uint8_t*,size_t,uint8_t*,size_t,int,digest_block*);

#endif /* FINALIZE_H */
