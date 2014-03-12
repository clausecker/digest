#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* first 64 primes */
static short primes[] = {
	  2,   3,   5,   7,  11,  13,  17,  19,
	 23,  29,  31,  37,  41,  43,  47,  53,
	 59,  61,  67,  71,  73,  79,  83,  89,
	 97, 101, 103, 107, 109, 113, 127, 131,
	137, 139, 149, 151, 157, 163, 167, 173,
	179, 181, 191, 193, 197, 199, 211, 223,
	227, 229, 233, 239, 241, 251, 257, 263,
	269, 271, 277, 281, 283, 293, 307, 311,
};

int main() {
	int i;
	double f;

	putchar('{');

	for (i = 0; i < 64; i++) {
		if (i % 4 == 0) printf("\n\t");
		f = pow(((double)primes[i]), 1.0/3.0);
		printf("0x%08x, ", (uint32_t)(f*4294967296.0));
	}

	puts("\n};");

	return EXIT_SUCCESS;
}
