#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROWS 24
#define COLS 40

uint8_t out[ROWS * COLS];

int main(void)
{
	uint8_t currentcode = 0;

	for(int i = 0; i < 1120; i++) {
		int c = fgetc(stdin);

		if(c == EOF) {
			fprintf(stderr, "premature EOF\n");
			return EXIT_FAILURE;
		}

		const char *map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
		const char *p = strchr(map, c);

		if(!p) {
			fprintf(stderr, "invalid character '%c'\n", c);
			return EXIT_FAILURE;
		}

		uint8_t dec = (uint8_t)(p - map);

		for(int b = 0; b < 6; b++) {
			uint8_t charbit = (6 * i + b) % 7;
			uint8_t b64bit = !!(dec & (1 << (5-b)));

			currentcode |= b64bit << (6 - charbit);

			if(charbit == 6) {
				uint16_t charnum = ((6 * i + b) - charbit) / 7;

				out[charnum] = currentcode;

				currentcode = 0;
			}
		}
	}

	for(int i = 0; i < ROWS; i++) {
		fwrite(&out[i * COLS], COLS, 1, stdout);
		puts("|");
	}

	return EXIT_SUCCESS;
}
