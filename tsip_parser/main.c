#include <stdio.h>
#include "tsip_parser.h"
#include <stdint.h>
#include <string.h>

static double fetch_f64( const void * ptr )
{
double retn;
uint64_t temp;

memcpy( &temp, ptr, sizeof(temp) );
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
temp = __builtin_bswap64( temp );
#endif
memcpy( &retn, &temp, sizeof(temp) );

return retn;
}

static float fetch_f32( const void * ptr )
{
float retn;
uint32_t temp;

memcpy( &temp, ptr, sizeof(temp) );
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
temp = __builtin_bswap32( temp );
#endif
memcpy( &retn, &temp, sizeof(temp) );

return retn;
}

static double fetch_deg( const void * ptr )
{
static const double ICD_GPS_200_PI = 3.1415926535898;
return fetch_f64( ptr ) * 180.0 / ICD_GPS_200_PI;
}

void tsip_process_packet( unsigned char id, const unsigned char * packet, unsigned len )
{
printf("PacketID:0x%02x|Len:%u|",id,len);
switch( id )
    {
    case 0x46:
        switch( packet[0] )
            {
            case 0:printf("doing position fixes");break;
            case 1:printf("do not have GPS time yet");break;
            case 2:printf("Need init");break;
            case 3:printf("PDOP too high");break;
            case 8:printf("No sats");break;
            case 9:printf("only 1 sat");break;
            case 10:printf("only 2 sats");break;
            case 11:printf("only 3 sats");break;
            case 12:printf("badsat");break;
            }
        break;
    case 0x84:
        printf("lat=%f,lon=%f",fetch_deg(packet), fetch_deg(packet+8));
        break;
    case 0x4B:
        printf("MachineID:0x%02x|RTC Available At Powerup:%i|Almanac not complete/current:%i",packet[0],!(packet[1]&2), !!(packet[1]&8) );
        break;
    case 0x6D:
        printf("Sat Selection|!Auto/Manual:%i|Mode:%iD|PRNS:%i",!!(packet[0]&0x8),(packet[0]&7)-1,len-16 );
        break;
    case 0x82:
        printf("%s DGPS(Differential %s)",packet[0]&2 ? "auto":"manual", packet[0]&1 ? "on":"off" );
        break;
    case 0x8F:
        printf("SubId:%i",packet[0]);
        break;
    case 0x56:
        printf("EastVel:%f|NorthVel:%f|UpVel:%f", fetch_f32(packet), fetch_f32(packet+4), fetch_f32(packet+8) );
        break;
    }
printf("\n");
}

int main( int nArgs, const char * const args[] )
{
int i;
for( i = 1; i < nArgs; ++i )
    {
    FILE * fp = fopen( args[i], "rb" );
    if( !fp )
        {
        fprintf(stderr,"unable to open:%s\n",args[i]);
        continue;
        }

    tsip_parser_reset();
    do
        {
        int ch = fgetc( fp );
        if( EOF == ch ) break;
        tsip_parser_push( ch );
        } while(1);
    }
}
