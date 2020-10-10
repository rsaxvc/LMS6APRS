/*	Simple task management
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef TASKS_H
#define TASKS_H

/*	Macros
 */
/*Task IDs are numbers 0-7*/
#define TASK_ID_GPS_FIX 0
#define TASK_ID_GPS_PKT 1
#define TASK_ID_APRS_TX 2
#define TASK_ID_GPS_OVR 3

/*Tasks are a bitfield of IDs*/
#define TASK_GPS_FIX (1<<(TASK_ID_GPS_FIX))
#define TASK_GPS_PKT (1<<(TASK_ID_GPS_PKT))
#define TASK_APRS_TX (1<<(TASK_ID_APRS_TX))
#define TASK_GPS_OVR (1<<(TASK_ID_GPS_OVR))

/*In this way, a bit-list of tasks can be pended for execution*/
void pend_task( unsigned char new_tasks );
void pend_task_irq( unsigned char new_tasks );
void task_aprs_tx( void);
void task_gps_fix( void );
void task_gps_pkt( void );
void task_gps_ovr( void );

#endif
