#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tables.h"

#define ROWS 24
#define COLS 40

uint8_t in[ROWS * COLS];

void write_pes_header(int data_unit_id, int field_parity, int line_offset)
{
	assert(line_offset == 0 || (line_offset >= 7 && line_offset <= 22));

	putchar(data_unit_id); // data_unit_id
	putchar(0x2C); // data_unit_length

	if(data_unit_id == 0x02) {
		// reserved_future_use, field_parity, line_offset
		putchar(0xC0 | (!!field_parity << 5) | (line_offset & 0x1F));
		putchar(0xE4); // framing code
	}
}

void write_stuffing(void) {
	write_pes_header(0xFF, 0, 0);

	int i = 0x2C;
	while(i--)
		putchar(0xFF);
}

void write_packet_address(int magazine, int packet_number)
{
	uint8_t nibble;
	nibble =
		!!(packet_number & 0x01) << 3 |
		!!(magazine & 0x04)      << 2 |
		!!(magazine & 0x02)      << 1 |
		!!(magazine & 0x01);

	putchar(invert[hamming[nibble]]);

	nibble =
		!!(packet_number & 0x10) << 3 |
		!!(packet_number & 0x08) << 2 |
		!!(packet_number & 0x04) << 1 |
		!!(packet_number & 0x02);

	putchar(invert[hamming[nibble]]);
}

void write_line(int magazine, int packet_number, unsigned char *data)
{
	write_packet_address(magazine, packet_number);

	for(int i = 0; i < 40; i++)
		putchar(invert[parity[data[i]]]);
}

#define ERASE_PAGE (1 << 0)
#define NEWSFLASH (1 << 1)
#define SUBTITLE (1 << 2)
#define SUPPRESS_HEADER (1 << 3)
#define UPDATE_INDICATOR (1 << 4)
#define INTERRUPTED_SEQUENCE (1 << 5)
#define INHIBIT_DISPLAY (1 << 6)
#define MAGAZINE_SERIAL (1 << 7)
#define C12 (1 << 8)
#define C13 (1 << 9)
#define C14 (1 << 10)

void write_header(int magazine, uint8_t page, uint16_t subcode, uint16_t control, unsigned char *data)
{
	write_packet_address(magazine, 0);

	putchar(invert[hamming[page & 0x0F]]); // page number units
	putchar(invert[hamming[page >> 4]]); // page number tens

	putchar(invert[hamming[subcode & 0x0F]]);

	uint8_t nibble;
	nibble =
		!!(control & ERASE_PAGE) << 3 |
		!!(subcode & (1 << 6))   << 2 |
		!!(subcode & (1 << 5))   << 1 |
		!!(subcode & (1 << 4));

	putchar(invert[hamming[nibble]]);

	nibble =
		!!(subcode & (1 << 10))   << 3 |
		!!(subcode & (1 <<  9))   << 2 |
		!!(subcode & (1 <<  8))   << 1 |
		!!(subcode & (1 <<  7));

	putchar(invert[hamming[nibble]]);

	nibble =
		!!(control & SUBTITLE)    << 3 |
		!!(control & NEWSFLASH)   << 2 |
		!!(subcode & (1 << 12))   << 1 |
		!!(subcode & (1 << 11));

	putchar(invert[hamming[nibble]]);

	nibble =
		!!(control & INHIBIT_DISPLAY)      << 3 |
		!!(control & INTERRUPTED_SEQUENCE) << 2 |
		!!(control & UPDATE_INDICATOR)     << 1 |
		!!(control & SUPPRESS_HEADER);

	putchar(invert[hamming[nibble]]);

	nibble =
		!!(control & C14) << 3 |
		!!(control & C13) << 2 |
		!!(control & C12) << 1 |
		!!(control & MAGAZINE_SERIAL);

	putchar(invert[hamming[nibble]]);

	for(int i = 0; i < 32; i++)
		putchar(invert[parity[data[i]]]);
}

int8_t hex2nibble(unsigned char c) {
	if('0' <= c && c <= '9') {
		return c - '0';
	} else {
		c |= 0x20;

		if('a' <= c && c <= 'f')
			return c - 'a' + 10;
	}

	fprintf(stderr, "invalid hex char '%c'\n", c);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	uint8_t magazine, page;
	if(argc < 2) {
		fprintf(stderr, "usage: %s page < in.plain > out.txt\n", argv[0]);
		return EXIT_FAILURE;
	}

	magazine = hex2nibble(argv[1][0]);

	page = hex2nibble(argv[1][1]) << 8 |
		hex2nibble(argv[1][2]);

	for(int i = 0; i < ROWS; i++) {
		fread(&in[i * COLS], COLS, 1, stdin);

		int c;
		do {
			c = getchar();
		} while(c != '\n');
	}

	write_pes_header(0x02, 1, 7);
	write_header(magazine, page, 0x0000, UPDATE_INDICATOR | MAGAZINE_SERIAL, &in[8]);
	for(int packet = 1; packet < 13; packet++) {
		write_pes_header(0x02, 1, 7 + packet);
		write_line(1, packet, &in[COLS * packet]);
	}

	write_stuffing();
	write_stuffing();

	for(int packet = 13; packet < 24; packet++) {
		write_pes_header(0x02, 0, 7 + (packet - 12));
		write_line(1, packet, &in[COLS * packet]);
	}

	write_stuffing();
	write_stuffing();
	write_stuffing();

	return EXIT_SUCCESS;
}
