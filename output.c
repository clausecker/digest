#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "output.h"

const char formats[] = "bclsx\0";

static void printpath(const char*);
static void digest2hex(char*,const uint8_t*,size_t);
static char *short2str(char[6],uint16_t);
static char *int2str(char[11],uint32_t);

typedef int formatter(const uint8_t*,size_t,off_t,const char*);
static formatter format_bsd;
static formatter format_posix;
static formatter format_sysv;
static formatter format_hex;
static formatter format_hexlen;

static formatter *formatters[] = {
	format_bsd,
	format_posix,
	format_hexlen,
	format_sysv,
	format_hex,
};

extern int format_digest(
	const uint8_t *p,
	size_t plen,
	off_t flen,
	const char *path,
	enum digest_format f) {

	return formatters[f](p, plen, flen, path);
}

static int format_hex(const uint8_t *p, size_t plen, off_t flen, const char *path) {
	char *hexdigest = alloca(2*plen + 1);
	(void)flen;

	digest2hex(hexdigest, p, plen);
	fputs(hexdigest, stdout);
	fputs("  ", stdout);
	if (*path == '\0') path = "-";
	puts(path);

	return 0;
}

static int format_hexlen(const uint8_t *p, size_t plen, off_t flen, const char *path) {
	char *hexdigest = alloca(2*plen + 1), buf[12] = "           \0";

	digest2hex(hexdigest, p, plen);
	fputs(hexdigest, stdout);
	putc(' ', stdout);
	int2str(buf, flen);
	fputs(buf, stdout);
	if (*path == '\0') path = "-";
	puts(path);

	return 0;
}

static int format_posix(const uint8_t *p, size_t plen, off_t flen, const char *path) {
	char buf[12] = "           \0";
	uint32_t digest;

	switch (plen) {
	case 4: digest = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3]; break;
	case 2: digest = p[0] << 8 | p[1]; break;
	default: fputs("Digest incompatible to output format\n", stderr); return 1;
	}

	fputs(int2str(buf, digest), stdout);
	buf[10] = '\0';
	fputs(int2str(buf, flen), stdout);
	printpath(path);

	return 0;
}

static int format_bsd(const uint8_t *p, size_t plen, off_t flen, const char *path) {
	char buf[7] = "00000 \0";

	if (plen != 2) {
		fputs("Digest incompatible to output format\n", stderr);
		return 1;
	}

	short2str(buf, p[0] << 8 | p[1]);
	fputs(buf, stdout);

	/* Implementation restriction: Can only hash files with up to 64 MiB */
	if (flen >= UINT16_MAX * 1024) {
		fputs("File too large for this format\n", stderr);
		fputs("?????", stdout);
		printpath(path);
		return 1;
	} else {
		flen = (flen + 1023) >> 10;
		memset(buf, ' ', 5);
		short2str(buf, flen);
		buf[5] = '\0';
		fputs(buf, stdout);
		printpath(path);
		return 0;
	}
}

static int format_sysv(const uint8_t *p, size_t plen, off_t flen, const char *path) {
	char buf[7];

	buf[5] = ' ';
	buf[6] = '\0';

	if (plen != 2) {
		fputs("Digest incompatible to output format\n", stderr);
		return 1;
	}

	fputs(short2str(buf, p[0] << 8 | p[1]), stdout);

	/* Implementation restriction: Can only hash files with up to 32 MiB */
	if (flen > UINT16_MAX * 512) {
		fputs("File too large for this format\n", stderr);
		fputs("?????", stdout);
		printpath(path);
		return 1;
	} else {
		flen = (flen + 511) >> 9;
		buf[5] = '\0';
		fputs(short2str(buf, flen), stdout);
		printpath(path);
		return 0;
	}
}

static void printpath(const char *path) {
	if (*path != '\0') {
		putchar(' ');
		puts(path);
	} else {
		putchar('\n');
	}
}

static void digest2hex(char *str, const uint8_t *buf, size_t len) {
	static const char hextab[16] = "0123456789abcdef";
	size_t i;

	for (i = 0; i < len; i++) {
		str[2*i] = hextab[buf[i] >> 4];
		str[2*i+1] = hextab[buf[i] & 0xf];
	}

	str[2*i] = '\0';
}

/* Doesn't terminate string */
static char *short2str(char str[6], uint16_t n) {
	int i = 5;

	do {
		str[--i] = '0' + n % 10;
		n /= 10;
	} while (n != 0);

	return str + i;
}

/* Dito */
static char *int2str(char str[11], uint32_t n) {
	int i = 10;

	do {
		str[--i] = '0' + n % 10;
		n /= 10;
	} while (n != 0);

	return str + i;
}
