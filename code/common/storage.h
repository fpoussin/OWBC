
#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "ch.h"
#include "common.h"

#define SETTINGS_START 0
#define SETTINGS_END 999
#define SETTINGS_LEN (SETTINGS_END-SETTINGS_START)
#define VERSIONS_START 991
#define VERSIONS_END 1023
#define VERSIONS_LEN (VERSIONS_END-VERSIONS_START)

#define VERSION_IDX_BL  0
#define VERSION_IDX_APP 1

extern uint32_t __user_settings_address__;
#define SETTINGS_ADDR (__user_settings_address__ + SETTINGS_START)
#define SETTINGS_PTR (uint32_t*)(SETTINGS_ADDR)
#define VERSION_ADDR (__user_settings_address__ + VERSIONS_START)
#define VERSION_PTR (uint32_t*)(VERSION_ADDR)

typedef uint32_t crc_t;

typedef struct {
    uint8_t api;
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} version_t;

/* 28 bytes data + 4 bytes crc */
typedef struct {
  uint32_t key;
  uint16_t cnt;
  version_t version;
  uint16_t functions;
  uint16_t canAddr;
  uint8_t outputMode;
  uint8_t refreshRate;
  uint8_t reserved[12];
  crc_t crc;
} settings_t;

/* Public functions */
uint8_t readVersionFromFlash(uint8_t idx, version_t* dst);
uint8_t writeVersionToFlash(uint8_t idx, const version_t* src);

extern version_t versions[2];
extern settings_t settings;

uint8_t readSettingsFromFlash(void);
uint8_t writeSettingsToFlash(void);

#endif
