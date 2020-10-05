/*	GPS Driver
 *	Copyright (c) 2020 by SecKC Software
 */
#ifndef GPS_H
#define GPS_H

void GPS_init(void);
void GPS_interrupt(void);

extern float gps_lla_packet[5]; /*Only valid for TASK_GPS_FIX */
extern unsigned char gps_pkt_id; /* Only valid for TASK_GPS_PKT */

#endif
