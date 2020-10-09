/*	CC1050 management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */
#ifndef GPIO_DEF_H
#define GPIO_DEF_H
#include <io72324.h>

#define GPIO_CC1050_LDO_PORT PDDR
#define GPIO_CC1050_LDO_PIN  5
#define GPOI_CC1050_LDO_BIT  (1<<GPIO_CC1050_LDO_PIN)

#define GPIO_CC1050_PALE_PORT PCDR
#define GPIO_CC1050_PALE_PIN  0
#define GPIO_CC1050_PALE_BIT  (1<<GPIO_CC1050_PALE_PIN)

#define GPIO_CC1050_PDATA_PORT PCDR
#define GPIO_CC1050_PDATA_PIN  5
#define GPIO_CC1050_PDATA_BIT  (1<<GPIO_CC1050_PDATA_PIN)

#define GPIO_CC1050_PCLK_PORT PCDR
#define GPIO_CC1050_PCLK_PIN  6
#define GPIO_CC1050_PCLK_BIT  (1<<GPIO_CC1050_PCLK_PIN)

#define GPIO_CC1050_DI_PORT PFDR
#define GPIO_CC1050_DI_PIN 4
#define GPIO_CC1050_DI_BIT (1<<GPIO_CC1050_DI_PIN)

#define GPIO_SOFTUART_TX_PORT PFDR
#define GPIO_SOFTUART_TX_PIN  6
#define GPIO_SOFTUART_TX_BIT (1<<GPIO_SOFTUART_TX_PIN)

#define GPIO_SET( PORT, PIN ) ((PORT) |=  (1<<(PIN)))
#define GPIO_CLR( PORT, PIN ) ((PORT) &= ~(1<<(PIN)))
#define GPIO_GET( PORT, PIN ) !!((PORT) & (1<<(PIN)))
#define GPIO_TGL( PORT, PIN ) ((PORT) ^=  (1<<(PIN)))

#endif