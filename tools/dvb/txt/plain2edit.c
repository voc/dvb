#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROWS 24
#define COLS 40

uint8_t in[ROWS * COLS];
uint8_t out[1120];

int main(void)
{
	for(int i = 0; i < ROWS; i++) {
		fread(&in[i * COLS], COLS, 1, stdin);

		int c;
		do {
			c = getchar();
		} while(c != '\n');
	}

	memset(out, 0, 1120);
	for(int i = 0; i < ROWS * COLS; i++) {
		for(int b = 0; b < 7; b++) {
			uint32_t framebit = 7 * i + b;

			uint8_t b64bitoffset = framebit % 6;
			uint32_t b64charoffset = (framebit - b64bitoffset) / 6;

			uint8_t bitval = !!(in[i] & (1 << (6 - b)));

			if(b64charoffset >= 1120) {
				fprintf(stderr, "offset too big\n");
				return EXIT_FAILURE;
			}

			out[b64charoffset] |= bitval << (5 - b64bitoffset);
		}
	}

	const char *map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	for(int i = 0; i < 1120; i++) {
		if(out[i] >= strlen(map)) {
			fprintf(stderr, "character 0x%02X out of bounds\n", out[i]);
			return EXIT_FAILURE;
		}

		putchar(map[out[i]]);
	}

	putchar('\n');

	return EXIT_SUCCESS;
}
