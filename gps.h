/*	GPS Driver
 *	Copyright (c) 2020 by SecKC Software
 */
#ifndef GPS_H
#define GPS_H

#include <stdint.h>

void GPS_init(void);
void GPS_interrupt(void);

extern float gps_lla_packet[5]; /*Only valid for TASK_GPS_FIX */
extern uint8_t gps_pkt_id; /* Only valid for TASK_GPS_PKT */
extern uint8_t gps_pkt_super; /* Only valid for TASK_GPS_PKT == superpacket*/
extern uint8_t gps_pkt_len;

/*FeatherHAB interfaces*/
char* get_longitudeTrimmed(void);
char* get_longitudeLSBs(void);
char* get_latitudeTrimmed(void);
char* get_latitudeLSBs(void);
char* get_timestamp(void);
char* get_gpsaltitude(void);
char* get_speedKnots(void);
char* get_course(void);
char* get_hdop(void);
char* get_sv(void);
char* get_dayofmonth(void);
#endif
