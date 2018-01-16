#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t hamming[] = {
	0x15, 0x02, 0x49, 0x5E, 0x64, 0x73, 0x38, 0x2F, 0xD0, 0xC7, 0x8C, 0x9B, 0xA1, 0xB6, 0xFD, 0xEA
};

uint8_t parity[] = {
	0x80, 0x01, 0x02, 0x83, 0x04, 0x85, 0x86, 0x07, 0x08, 0x89, 0x8a, 0x0b, 0x8c, 0x0d, 0x0e, 0x8f,
	0x10, 0x91, 0x92, 0x13, 0x94, 0x15, 0x16, 0x97, 0x98, 0x19, 0x1a, 0x9b, 0x1c, 0x9d, 0x9e, 0x1f,
	0x20, 0xa1, 0xa2, 0x23, 0xa4, 0x25, 0x26, 0xa7, 0xa8, 0x29, 0x2a, 0xab, 0x2c, 0xad, 0xae, 0x2f,
	0xb0, 0x31, 0x32, 0xb3, 0x34, 0xb5, 0xb6, 0x37, 0x38, 0xb9, 0xba, 0x3b, 0xbc, 0x3d, 0x3e, 0xbf,
	0x40, 0xc1, 0xc2, 0x43, 0xc4, 0x45, 0x46, 0xc7, 0xc8, 0x49, 0x4a, 0xcb, 0x4c, 0xcd, 0xce, 0x4f,
	0xd0, 0x51, 0x52, 0xd3, 0x54, 0xd5, 0xd6, 0x57, 0x58, 0xd9, 0xda, 0x5b, 0xdc, 0x5d, 0x5e, 0xdf,
	0xe0, 0x61, 0x62, 0xe3, 0x64, 0xe5, 0xe6, 0x67, 0x68, 0xe9, 0xea, 0x6b, 0xec, 0x6d, 0x6e, 0xef,
	0x70, 0xf1, 0xf2, 0x73, 0xf4, 0x75, 0x76, 0xf7, 0xf8, 0x79, 0x7a, 0xfb, 0x7c, 0xfd, 0xfe, 0x7f,
};

uint8_t invert[] = {
	0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
	0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
	0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
	0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
	0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
	0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
	0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
	0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
	0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
	0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
	0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
	0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
	0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
	0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
	0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
	0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF };

uint8_t unhamming(uint8_t a)
{
	a = invert[a];

	for(uint8_t i = 0; i <= 0xf; i++)
		if(hamming[i] == a)
			return i;

	printf("hamming failed\n");
	exit(1);
}

uint32_t unhamming24(uint8_t *a)
{
	uint8_t b1, b2, b3;
	b1 = invert[*a++];
	b2 = invert[*a++];
	b3 = invert[*a++];

	uint32_t out = 0;

	out =
		!!(b1 & (1 << 2)) <<  0 |
		!!(b1 & (1 << 4)) <<  1 |
		!!(b1 & (1 << 5)) <<  2 |
		!!(b1 & (1 << 6)) <<  3 |
		(b2 & 0x7f) <<  4 |
		(b3 & 0x7f) << 11;

	return out;
}

void hexdump(const uint8_t *buf, int len, uint8_t coding) {
    int i;
    unsigned char buff[17];
    unsigned char parity_buff[17];
    unsigned char pc;

    printf("\t\t\t");

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0) {
		printf("  ");
		for(int i = 0; buff[i]; i++) {
			if(parity_buff[i])
				putchar(buff[i]);
			else
				printf("\033[4m%c\033[0m", buff[i]);
		}
		printf("\n\t\t\t");
	    }

            // Output the offset.
            printf ("  %04x ", i);
        }

	uint8_t parity_valid = 1;
	if(coding == 0)
		pc = buf[i];
	else if(coding == 1)
		pc = unhamming(buf[i]);
	else if(coding == 2) {
		uint8_t x = invert[buf[i]];

		if(parity[x & 0x7f] != x)
			parity_valid = 0;

		pc = x & 0x7f;
	}

        // Now the hex code for the specific character.
	if(parity_valid)
		printf (" %02x", pc);
	else
		printf(" \033[4m%02x\033[0m", pc);

        // And store a printable ASCII character for later.
        if ((pc < 0x20) || (pc > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc;
	parity_buff[i % 16] = parity_valid;

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
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

int main(int argc, char **argv)
{
	int magazine_filter = -1;

	if(argc > 1)
		magazine_filter = atoi(argv[1]);

	uint32_t offset = -1;
	while(1) {
		int c = getchar();
		offset++;

		if(c == EOF)
			return 0;

		uint8_t meta;

		if(c == 0x02) {
			c = getchar();
			offset++;

			if(c != 0x2c) {
				//printf("skipping 2 (%d / %x)\n", offset, offset);
				continue;
			}

			meta = getchar();
			offset++;

			c = getchar();
			offset++;
			if(c != 0xe4) {
				//printf("skipping 3 (%d / %x)\n", offset, offset);
				continue;
			}
		} else {
			//printf("skipping 1 (%d / %x)\n", offset, offset);
			continue;
		}

		unsigned char buf[42];
		fread(buf, 42, 1, stdin);
		offset += 42;

		uint8_t field_parity, line_offset;
		field_parity = (meta >> 5) & 1;
		line_offset = meta & 0x1f;

		uint8_t magazine, packet;
		magazine = unhamming(buf[0]) & 7;
		packet = unhamming(buf[1]) << 1 | !!(unhamming(buf[0]) & 8);

		if(magazine_filter >= 0 && magazine != magazine_filter)
			continue;

		printf("---------\n");
		printf("parity: %d, line_offset: %02d (%d)\n", field_parity, line_offset,
				field_parity ? line_offset : (line_offset + 313));
		printf("magazine: 0x%x\npacket: %d\n", magazine, packet);

		if(packet == 0) {
			printf("type: header\n");

			uint8_t page;
			uint16_t subcode = 0;
			uint16_t control = 0;

			page = unhamming(buf[3]) << 4 | unhamming(buf[2]);

			uint8_t nibble;
			nibble = unhamming(buf[4]);
			subcode |= nibble;

			nibble = unhamming(buf[5]);
			subcode |= (nibble & 0x7) << 4;
			control |= (nibble & 0x8) ? ERASE_PAGE : 0;

			nibble = unhamming(buf[6]);
			subcode |= nibble << 7;

			nibble = unhamming(buf[7]);
			subcode |= (nibble & 0x3) << 11;
			control |= (nibble & 0x8) ? SUBTITLE : 0;
			control |= (nibble & 0x4) ? NEWSFLASH : 0;

			nibble = unhamming(buf[8]);
			control |= (nibble & 0x8) ? INHIBIT_DISPLAY : 0;
			control |= (nibble & 0x4) ? INTERRUPTED_SEQUENCE : 0;
			control |= (nibble & 0x2) ? UPDATE_INDICATOR : 0;
			control |= (nibble & 0x1) ? SUPPRESS_HEADER : 0;

			nibble = unhamming(buf[9]);
			control |= (nibble & 0x8) ? C14 : 0;
			control |= (nibble & 0x4) ? C13 : 0;
			control |= (nibble & 0x2) ? C12 : 0;
			control |= (nibble & 0x1) ? MAGAZINE_SERIAL : 0;

			printf("page number: 0x%02X\n", page);
			printf("subcode: 0x%04X\n", subcode);
			printf("control: 0x%04X\n", control);

#define PRINT_MASK(val, bit) if((val) & bit) printf("  " #bit "\n")
			PRINT_MASK(control, ERASE_PAGE);
			PRINT_MASK(control, SUBTITLE);
			PRINT_MASK(control, NEWSFLASH);
			PRINT_MASK(control, INHIBIT_DISPLAY);
			PRINT_MASK(control, UPDATE_INDICATOR);
			PRINT_MASK(control, SUPPRESS_HEADER);
			PRINT_MASK(control, C14);
			PRINT_MASK(control, C13);
			PRINT_MASK(control, C12);
			PRINT_MASK(control, MAGAZINE_SERIAL);

			hexdump(&buf[10], 32, 2);
		} else if(packet >= 1 && packet <= 25) {
			printf("type: line\n");

			hexdump(&buf[2], 40, 2);
		} else if(packet == 26 || packet == 28 || packet == 29) {
			printf("type: page enhancement (%d)\n", packet);

			printf("designation code: %d\n", unhamming(buf[2]));

			for(int i = 1; i <= 13; i++) {
				printf("triplet %d: 0x%06x\n", i, unhamming24(&buf[3 * i]));
			}
		} else if(packet == 27) {
			printf("type: linking\n");

			printf("designation code: %d\n", unhamming(buf[2]));
		} else if(packet == 30 || packet == 31) {
			printf("type: independent data\n");
		}

		printf("\n");
	}
}
