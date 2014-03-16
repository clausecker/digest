#include <alloca.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adler32.h"
#include "bsd.h"
#include "cksum.h"
#include "crc32.h"
#include "md5.h"
#include "sha1.h"
#include "sha2.h"
#include "sysv.h"
#include "output.h"

/* list of all available digest modules. Remove modules to save space. */
static const struct digest_descr digests[] = {
	DIGEST_DESCR(0,'x',"adler32",ADLER32,adler32),
	DIGEST_DESCR(0,'b',"bsd",BSD,bsd),
	DIGEST_DESCR(0,'c',"cksum",CKSUM,cksum),
	DIGEST_DESCR(0,'x',"crc32",CRC32,crc32),
	DIGEST_DESCR(0,'x',"md5",MD5,md5),
	DIGEST_DESCR(0,'x',"sha1",SHA1,sha1),
	DIGEST_DESCR(0,'x',"sha224",SHA224,sha224),
	DIGEST_DESCR(0,'x',"sha256",SHA256,sha256),

	/* duplicates for compatibility with other tools */
	DIGEST_DESCR(1,'b',"sum",BSD,bsd), /* compatibility with sum */
	DIGEST_DESCR(1,'x',"sha",SHA1,sha1), /* compatibilit with shasum */
	DIGEST_DESCR(1,'x',"1",SHA1,sha1),
	DIGEST_DESCR(1,'x',"224",SHA224,sha224),
	DIGEST_DESCR(1,'x',"256",SHA256,sha256),

	/* table may not end with a hidden entry */
	DIGEST_DESCR(0,'s',"sysv",SYSV,sysv),
};

enum {
	DIGESTCOUNT = sizeof digests / sizeof*digests,
};

static const struct digest_descr *get_digest(const char*,const char*);
static void help(const char*);
static int file_digest(const struct digest_descr*,const char*,enum digest_format);

int main(int argc, char **argv) {
	const struct digest_descr *digest;
	int c, i, error = EXIT_SUCCESS, fchar = 0;
	const char *algorithm = NULL, *fptr;
	enum digest_format f;

	while ((c = getopt(argc, argv, "a:f:hrs")) != EOF) switch (c) {
		case 'a': algorithm = optarg; break;
		case 'f': fchar = *optarg; break;
		case 'h': help(argv[0]); return 0;
		case 'r': algorithm = "bsd"; break;
		case 's': algorithm = "sysv"; break;
		case '?': help(argv[0]); return 1;
	};

	digest = get_digest(argv[0], algorithm);

	if (digest == NULL) {
		fputs("Cannot figure out what algorithm you want. Try passing -a algorithm.\n", stderr);
		help(argv[0]);
		return EXIT_FAILURE;
	}

	if (fchar == 0) fchar = digest->fchar;
	if (NULL == (fptr = strchr(formats, fchar))) {
		fputs("Unknown format character.\n", stderr);
		return EXIT_FAILURE;
	}

	f = (enum digest_format)(fptr - formats);

	/* no file names provided, read stdin */
	if (argc <= optind) {
		return file_digest(digest, NULL, f);
	}

	for (i = optind; i < argc; i++)
		if (file_digest(digest, argv[i], f)) error = EXIT_FAILURE;

	return error;
}

static const struct digest_descr *get_digest(const char *program, const char *name) {
	const char *base = strrchr(program,'/');
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
	fputs(" [-hrs] [-f bclsx] [-a algorithm] file...\nAvailable algorithms: ", stderr);
	for (i = 0; i < DIGESTCOUNT - 1; i++) if (!digests[i].hide) {
		fputs(digests[i].name, stderr);
		fputs(", ", stderr);
	}

	fputs(digests[i].name, stderr);
	putc('\n', stderr);
}

static int file_digest(const struct digest_descr *d, const char *filename, enum digest_format f) {
	FILE *file;
	size_t len, flen = 0;
	union digest_state *state;
	uint8_t *block, *digest;
	int err;

	if (filename != NULL && strcmp(filename, "-"))
		file = fopen(filename, "rb");
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
	while (d->blocklen == (len = fread(block, 1, d->blocklen, file))) {
		d->block(state, block);
		flen += len;
	}

	flen += len;

	if (!feof(file)) {
		fputs("Error reading file ", stderr);
		fputs(filename, stderr);
		putc('\n', stderr);
		return 1;
	}

	digest = alloca(d->digestlen);
	d->final(state, block, len, digest);

	if (filename == NULL) filename = "";
	err = format_digest(digest, d->digestlen, flen, filename, f);

	if (strcmp(filename, "-")) fclose(file);

	return err;
}
