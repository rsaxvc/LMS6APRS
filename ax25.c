/*
 * FeatherHAB 
 *
 * This file is part of FeatherHAB.
 *
 * FeatherHab is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FeatherHab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with FeatherHAB. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Ethan Zonca
 *
 */
 
 /* 9600-baud FSK bits from trackuino copyright (C) 2010  EA5HAV Javi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "config.h"
#include "ax25.h"
#include "afsk_mod.h"

// Module globals
static uint16_t crc;
static uint8_t ones_in_a_row;
static uint8_t packet[MAX_PACKET_LEN];
static unsigned int packet_size;

// Private
static void update_crc(uint8_t a_bit);
static void send_byte(uint8_t a_byte);

// Module functions
static void update_crc(uint8_t a_bit)
{
	crc ^= a_bit;
	if (crc & 1)
        crc = (crc >> 1) ^ 0x8408;  // X-modem CRC poly
	else
        crc = crc >> 1;
}

static void send_byte(uint8_t a_byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		uint8_t a_bit = a_byte & 1;
		a_byte >>= 1;
		update_crc(a_bit);
		if (a_bit) 
		{
			// Next bit is a '1'
			if (packet_size >= MAX_PACKET_LEN * 8)  // Prevent buffer overrun
                return;
			packet[packet_size >> 3] |= (1 << (packet_size & 7));
			packet_size++;
			if (++ones_in_a_row < 5) continue;
		}
		// Next bit is a '0' or a zero padding after 5 ones in a row
		if (packet_size >= MAX_PACKET_LEN * 8)    // Prevent buffer overrun
            return;
		packet[packet_size >> 3] &= ~(1 << (packet_size & 7));
		packet_size++;
		ones_in_a_row = 0;
	}
}

// Exported functions
void ax25_send_byte(uint8_t a_byte)
{
	// Wrap around send_byte, but prints debug info
	send_byte(a_byte);
}

void ax25_send_flag()
{
	uint8_t flag = 0x7e;
	int i;
	for (i = 0; i < 8; i++, packet_size++) 
	{
		if (packet_size >= MAX_PACKET_LEN * 8)  // Prevent buffer overrun
            return;
		if ((flag >> i) & 1)
            packet[packet_size >> 3] |= (1 << (packet_size & 7));
		else
            packet[packet_size >> 3] &= ~(1 << (packet_size & 7));
	}
}

void ax25_send_string(const char *string)
{
	int i;
	for (i = 0; string[i]; i++) 
	{
		ax25_send_byte(string[i]);
	}
}

void ax25_send_header(const struct s_address *addresses, uint16_t num_addresses)
{
	int i, j;
	packet_size = 0;
	ones_in_a_row = 0;
	crc = 0xffff;
	
#if MOD_AFSK
	// Send flags during TX_DELAY milliseconds (8 bit-flag = 8000/1200 ms)
	for (i = 0; i < TX_DELAY * 3 / 20; i++) 
	{
		ax25_send_flag();
	}
#elif MOD_FSK
	// Send flags during TX_DELAY milliseconds
	for (i = 0; i < TX_DELAY * 6 / 5; i++)
	{
		ax25_send_flag();
	}
#else
	#error
#endif

	for (i = 0; i < num_addresses; i++) 
	{
		// Transmit callsign
		for (j = 0; addresses[i].callsign[j]; j++)
            send_byte(addresses[i].callsign[j] << 1);
		// Transmit pad
		for ( ; j < 6; j++)
            send_byte(' ' << 1);
		// Transmit SSID. Termination signaled with last bit = 1
		if (i == num_addresses - 1)
            send_byte(('0' + addresses[i].ssid) << 1 | 1);
		else
            send_byte(('0' + addresses[i].ssid) << 1);
	}
	
	// Control field: 3 = APRS-UI frame
	send_byte(0x03);
	
	// Protocol ID: 0xf0 = no layer 3 data
	send_byte(0xf0);
}

void ax25_send_footer()
{
	// Save the crc so that it can be treated it atomically
	uint16_t final_crc = crc;
	
	// Send the CRC
	send_byte(~(final_crc & 0xff));
	final_crc >>= 8;
	send_byte(~(final_crc & 0xff));
	
	// Signal the end of frame
	ax25_send_flag();
}

static uint32_t lfsr;

static uint8_t scramble_bit(uint8_t _in) {
	uint8_t x = (_in ^ (lfsr >> 16) ^ (lfsr >> 11)) & 1;
	lfsr = (lfsr << 1) | (x & 1);
	return x;
}

/**
  * Scrambling for FSK
  */
static void scramble(void) {
	uint16_t i;
	// Scramble
	lfsr = 0;
	for(i=0; i<packet_size; i++) {
		if(scramble_bit((packet[i >> 3] >> (i & 0x7)) & 0x1)) {
			packet[i>> 3] |= (uint8_t)(1 << (i & 7));
		} else {
			packet[i>> 3] &= (uint8_t)~(1 <<(i & 7));
		}
	}
}

//The AFSK modulator does NRZI at the modem.
//The FSK modulator needs NRZI applied ahead of time
static void nrzi_encode(void) {
	uint16_t i;
	uint8_t ctone = 0;
	for( i=0; i<packet_size; i++) {
		if(((packet[i >> 3] >> (i & 0x7)) & 0x1) == 0)
			ctone = !ctone;
		if(ctone) {
			packet[i>> 3] |= (uint8_t)(1 << (i & 7));
		} else {
			packet[i>> 3] &= (uint8_t)~(1 <<(i & 7));
		}
	}
}

void ax25_flush_frame(void)
{
	#if MOD_FSK
		scramble();
		nrzi_encode();
	#endif
	putstr("Have ");put_hex_u16(packet_size);puts(" bits");
	
	// Key the transmitter and send the frame
	afsk_send(packet, packet_size);
}

// vim:softtabstop=4 shiftwidth=4 expandtab 