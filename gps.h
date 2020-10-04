/*	GPS Driver
 *	Copyright (c) 2020 by SecKC Software
 */
#ifndef GPS_H
#define GPS_H

void GPS_init(void);
void GPS_interrupt(void);

extern float gps_lla_packet[5];

#endif
