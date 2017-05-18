
#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "ch.h"
#include "common.h"

// EEPROM is M95640 (ST)
#define EEPROM_SIZE 8192 // 64Kb, 8KB
#define EEPROM_PAGE_SIZE 32
#define EEPROM_WRITE_TIME_MS 7 // 5ms byte/page write in datasheet
#define EEPROM_SPID SPID3
#define EEPROM_SPIDCONFIG spi3cfg

#define EEPROM_SETTINGS_START 0
#define EEPROM_SETTINGS_END 999
#define EEPROM_VERSIONS_START 1000
#define EEPROM_VERSIONS_END 1023

#define VERSION_IDX_BL  0
#define VERSION_IDX_APP 1

extern const SPIConfig EEPROM_SPIDCONFIG;

typedef struct {
    uint8_t api;
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} version_t;

typedef struct {
  uint32_t key;
  version_t version;
  uint16_t functions;
  uint16_t canAddr;
  uint8_t outputMode;
  uint8_t refreshRate;
} settings_t;

/* Public functions */
void eeInit(void);

uint8_t readVersionFromEE(uint8_t idx, version_t* dst);
uint8_t writeVersionToEE(uint8_t idx, const version_t* src);

extern version_t versions[2];
extern settings_t settings;

uint8_t readSettingsFromEE(void);
uint8_t writeSettingsToEE(void);

#endif
