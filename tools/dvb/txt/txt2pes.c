#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define VBIS_PER_PACKET 15

#define VBI_SIZE 46
#define PES_SIZE ((VBIS_PER_PACKET + 1) * VBI_SIZE)

/*
 * See ETSI EN 301 775 Sections 4.3 and 4.5 for the values used
 */

int main(int argc, char **argv)
{
	uint8_t pes_header[VBI_SIZE];

	memset(pes_header, 0xFF, VBI_SIZE);

	// packet_start_code_prefix
	pes_header[0] = 0x00;
	pes_header[1] = 0x00;
	pes_header[2] = 0x01;

	// stream_id = private_stream_1
	pes_header[3] = 0xBD;

	// pes_header_length
	uint16_t pes_header_length = PES_SIZE - 6;
	pes_header[4] = (pes_header_length >> 8) & 0xFF;
	pes_header[5] = pes_header_length & 0xFF;

	// flags: data_alignment_indicator = 1
	pes_header[6] = 0x84;
	// flags: PTS is set
	pes_header[7] = 0x80;

	// PES_header_data_length = 0x24
	pes_header[8] = 0x24;

	// data_identifier in PES_data_field
	pes_header[45] = 0x10;

	int vbis = 0;
	unsigned char buf[VBI_SIZE];
	while(1) {
		if(vbis % VBIS_PER_PACKET == 0)
			fwrite(pes_header, 1, sizeof(pes_header), stdout);

		size_t bytes_read = fread(buf, 1, VBI_SIZE, stdin);

		if(bytes_read == 0)
			break;

		if(bytes_read != VBI_SIZE) {
			fprintf(stderr, "short read: expected %u, got %zu\n", VBI_SIZE, bytes_read);
			break;
		}

		fwrite(buf, 1, VBI_SIZE, stdout);
		vbis++;
	}

	vbis %= VBIS_PER_PACKET;

	buf[0] = 0xFF; // data_unit_id
	buf[1] = 0x2C; // data_unit_length
	memset(buf+2, 0xFF, VBI_SIZE-2); // stuffing_byte

	while(vbis > 0 && vbis < VBIS_PER_PACKET) {
		fwrite(buf, 1, VBI_SIZE, stdout);
		vbis++;
	}

	return EXIT_SUCCESS;
}
