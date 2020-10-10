#ifndef CONFIG_H
#define CONFIG_H

// --------------------------------------------------------------------------
// AX.25 config (ax25.c)
// --------------------------------------------------------------------------

// TX delay in milliseconds
#define TX_DELAY      200

// Maximum packet delay
#define MAX_PACKET_LEN 512  // bytes

// --------------------------------------------------------------------------
// APRS config (aprs.c)
// --------------------------------------------------------------------------

// Set your callsign and SSID here. Common values for the SSID are
// (from http://zlhams.wikidot.com/aprs-ssidguide):
//
// - Balloons:  11
// - Cars:       9
// - Home:       0
// - IGate:      5
#define S_CALLSIGN      "KD0LIX"
#define S_CALLSIGN_ID   11

// Destination callsign: APRS (with SSID=0) is usually okay.
#define D_CALLSIGN      "APRS"
#define D_CALLSIGN_ID   0

// Digipeating paths:
// (read more about digipeating paths here: http://wa8lmf.net/DigiPaths/ )
// The recommended digi path for a balloon is WIDE2-1 or pathless. The default
// is pathless. Uncomment the following two lines for WIDE2-1 path:
#define DIGI_PATH1      "WIDE2"
#define DIGI_PATH1_TTL  1

// Transmit the APRS sentence every X milliseconds
#define APRS_TRANSMIT_PERIOD 60123


#endif

// vim:softtabstop=4 shiftwidth=4 expandtab 