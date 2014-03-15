#ifndef OUTPUT_H
#define OUTPUT_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* string with all available formats */
extern const char formats[];

/* digest formats as a list of indices into formats[] */
enum digest_format {
	/* b */ BSD_FORMAT,
	/* c */ POSIX_FORMAT,
	/* s */ SYSV_FORMAT,
	/* x */ HEX_FORMAT,
	/* y */ HEXLEN_FORMAT,
	/* 0 */ UNKNOWN_FORMAT,
};

extern int format_digest(const uint8_t*,size_t,off_t,const char*,enum digest_format);

#endif /* OUTPUT_H */
