#ifndef COMMON_H
#define COMMON_H

#include "ch.h"
#include "hal.h"
#include <stdio.h>

#define CCM_FUNC __attribute__((section(".ram4_init.code")))

#ifdef SEMIHOSTING
  #define DEBUGEN(x) if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) { x; }
#else
  #define DEBUGEN(x)
#endif

#define DBG_STREAM ((BaseSequentialStream *)&SD1)
#define MCU_UUID ((uint32_t*)0x1FFFF7AC)

uint32_t getuuid32(void);
uint16_t rand16(uint16_t min, uint16_t max);
uint32_t rand32(uint32_t min, uint32_t max);
uint32_t leToUInt32(uint8_t *ptr);
uint16_t leToUInt16(uint8_t *ptr);
uint32_t beToUInt32(uint8_t *ptr);
uint16_t beToUInt16(uint8_t *ptr);
uint8_t checksum(const uint8_t *data, uint8_t length);

bool getSwitch1(void);
int map(int x, int in_min, int in_max, int out_min, int out_max);


#endif
