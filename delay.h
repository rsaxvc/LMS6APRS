#ifndef DELAY_H
#define DELAY_H

/*Busy-wait delays*/
/*Simple CPU-based delays*/
/*may run slow during periods of heavy interupt processing*/
void delay_seconds( unsigned char sec );
void delay_millis( unsigned char ms );
void delay_micros( unsigned char us );

#endif