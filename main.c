#include <alloca.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adler32.h"
#include "bsd.h"
#include "digest.h"
#include "crc32.h"
#include "md5.h"
#include "sha1.h"
#include "sha2.h"

/* list of all available digest modules. Remove modules to save space. */
static const struct digest_descr digests[] = {
	DIGEST_DESCR(ADLER32,adler32),
	DIGEST_DESCR(BSD,bsd),
	DIGEST_DESCR(CRC32,crc32),
	DIGEST_DESCR(MD5,md5),
	DIGEST_DESCR(SHA1,sha1),
	DIGEST_DESCR(SHA224,sha224),
	DIGEST_DESCR(SHA256,sha256),
};

enum {
	BUFLEN = 4096,
	DIGESTCOUNT = sizeof digests / sizeof*digests,
};

static const struct digest_descr *get_digest(char*,char*);
static void help(const char*);
static int file_digest(const struct digest_descr*,const char*);
static void digest2str(const uint8_t*,char*,size_t);

int main(int argc, char **argv) {
	const struct digest_descr *digest;
	int i, error = EXIT_SUCCESS;
	char *digestname = NULL;
	int c;

	while ((c = getopt(argc, argv, "d:h")) != EOF) switch (c) {
		case '?': /* fallthrough */
		case 'h': help(argv[0]); return 0;
		case 'd': digestname = optarg; break;
	};

	digest = get_digest(argv[0], digestname);

	if (digest == NULL) {
		fputs("Cannot figure out what digest you want. Try passing -d <digest>\n", stderr);
		help(argv[0]);
		return 1;
	}

	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	for (i = optind; i < argc; i++)
		if (file_digest(digest, argv[i])) error = EXIT_FAILURE;

	return error;
}

static const struct digest_descr *get_digest(char *program, char *name) {
	char *base = strrchr(program,'/');
	int i;

	if (base++ == NULL) base = program;

	for (i = 0; i < DIGESTCOUNT; i++) {
		if (name && !strcmp(name, digests[i].name)) return &digests[i];
		if (!name && strstr(base, digests[i].name) == base) return &digests[i];
	}

	return NULL;
}

static void help(const char *program) {
	int i;

	fputs("Usage: ", stderr);
	fputs(program, stderr);
	fputs(" [-h | -d <digest>] file...\nAvailable digests: ", stderr);
	for (i = 0; i < DIGESTCOUNT - 1; i++) {
		fputs(digests[i].name, stderr);
		fputs(", ", stderr);
	}

	fputs(digests[i].name, stderr);
	putc('\n', stderr);
}

static int file_digest(const struct digest_descr *d, const char *filename) {
	FILE *file;
	size_t len;
	union digest_state *state;
	uint8_t *block, *digest;
	char *str;

	if (strcmp(filename, "-")) file = fopen(filename, "rb");
	else file = stdin;

	if (file == NULL) {
		fputs("Cannot open file ", stderr);
		fputs(filename, stderr);
		putc('\n', stderr);
		return 1;
	}

	state = alloca(d->statelen);
	memcpy(state, d->init, d->statelen);

	block = alloca(d->blocklen);
	while (d->blocklen == (len = fread(block, 1, d->blocklen, file)))
		d->block(state, block);

	if (!feof(file)) {
		fputs("Error reading file ", stderr);
		fputs(filename, stderr);
		putc('\n', stderr);
		return 1;
	}

	digest = alloca(d->digestlen);
	d->final(state, block, len, digest);

	str = alloca(d->digestlen*2 + 1);
	digest2str(digest, str, d->digestlen);
	fputs(str, stdout);
	fputs("  ", stdout);
	puts(filename);

	if (strcmp(filename, "-")) fclose(file);

	return 0;
}

static void digest2str(const uint8_t *buf, char *str, size_t len) {
	static const char hextab[16] = "0123456789abcdef";
	size_t i;

	for (i = 0; i < len; i++) {
		str[2*i] = hextab[buf[i] >> 4];
		str[2*i+1] = hextab[buf[i] & 0xf];
	}

	str[2*i] = '\0';
}
