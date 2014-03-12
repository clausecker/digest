#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
	int i;
	double f;

	putchar('{');

	for (i = 0; i < 64; i++) {
		if (i % 4 == 0) printf("\n\t");
		f = fabs(sin(i + 1));
		printf("0x%08x, ", (uint32_t)(f*4294967296.0));
	}

	puts("\n};");

	return EXIT_SUCCESS;
}
