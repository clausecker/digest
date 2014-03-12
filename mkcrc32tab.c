#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum {
	/* IEEE polynomial */
	CRC32_POLY = 0xedb88320,
};

uint32_t make_entry(uint32_t crc) {
	int j;
	for (j = 0; j < 8; j++)
		if (crc&1) crc = (crc >> 1) ^ CRC32_POLY; else crc >>= 1;

	return crc;
}

int main() {
	int i;

	putchar('{');

	for (i = 0; i < 256; i++) {
		if (i % 4 == 0) printf("\n\t");
		printf("0x%08x, ", make_entry(i));
	}

	puts("\n};");

	return EXIT_SUCCESS;
}
