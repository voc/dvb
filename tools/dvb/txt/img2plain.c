#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROWS 24
#define COLS 40

#define PPR 3
#define PPC 2

#define PIXROWS (ROWS * PPR)
#define PIXCOLS (COLS * PPC)

uint8_t in[PIXROWS * PIXCOLS];

uint8_t getpx(int r, int c)
{
	if(in[PIXCOLS * r  + c] != ' ')
		return 1;
	else
		return 0;
}

int main(void)
{
	memset(in, ' ', PIXROWS * PIXCOLS);

	int row = 0, col = 0;
	while(1) {
		int c = getchar();

		if(c == EOF)
			break;

		if(c == '\n') {
			row++;
			col = 0;
			continue;
		}

		if(row >= PIXROWS)
			break;

		if(col >= PIXCOLS)
			continue;

		in[row * PIXCOLS + col] = c;
		col++;
	}

	for(int row = 0; row < ROWS; row++) {
		uint8_t out[COLS];
		memset(out, ' ', COLS);

		out[0] = 0x17;

		for(int col = 0; col < COLS - 1; col++) {
			uint8_t val;

			val =
				getpx(PPR * row + 0, PPC * col + 0) << 0 | getpx(PPR * row + 0, PPC * col + 1) << 1 |
				getpx(PPR * row + 1, PPC * col + 0) << 2 | getpx(PPR * row + 1, PPC * col + 1) << 3 |
				getpx(PPR * row + 2, PPC * col + 0) << 4 | getpx(PPR * row + 2, PPC * col + 1) << 6;

			out[col + 1] = val + 0x20;
		}

		fwrite(out, COLS, 1, stdout);
		puts("|");
	}

	return EXIT_SUCCESS;
}
