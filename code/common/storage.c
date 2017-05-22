#include "storage.h"
#include "stm32f30x_flash.h"
#include <string.h>

static const uint32_t magic_key = 0xABEF1289;
settings_t settings_buf;

// Default settings
settings_t settings = {};

#if defined(MODE_APP)
version_t versions[2] = {{0, 0, 0, 0},
                         {VERSION_API, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH}};
#else
version_t versions[2] = {{VERSION_API, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH},
                         {0, 0, 0, 0}};
#endif


static const CRCConfig crc32_config = {
  .poly_size         = 32,
  .poly              = 0x04C11DB7,
  .initial_val       = 0xFFFFFFFF,
  .final_val         = 0xFFFFFFFF,
  .reflect_data      = 1,
  .reflect_remainder = 1
};

CCM_FUNC static crc_t getCrc(const CRCConfig *config, uint8_t *data, uint16_t len)
{
  uint32_t crc;

  crcAcquireUnit(&CRCD1);             /* Acquire ownership of the bus.    */
  crcStart(&CRCD1, config);           /* Activate CRC driver              */
  crcReset(&CRCD1);
  crc = crcCalc(&CRCD1, len, data);
  crcStop(&CRCD1);                    /* Deactive CRC driver);            */
  crcReleaseUnit(&CRCD1);             /* Acquire ownership of the bus.    */

  return crc;
}

CCM_FUNC static uint32_t findCurrentPageAddr(void)
{
    uint32_t addr;
    const uint32_t from = SETTINGS_ADDR;
    const uint32_t to = from + SETTINGS_LEN;
    settings_t * st;

    // Find highest counter
    for (addr = from; addr < to; addr += sizeof(settings_t))
    {

        st = (settings_t*)addr;
        if (st->key != magic_key)
        {
            // Return last good 'page'
            return addr - sizeof(settings_t);
        }
    }

    return from;
}

CCM_FUNC static uint32_t findNextPageAddr(void)
{
    uint32_t addr = findCurrentPageAddr();

    /* Check if at last page */
    if(addr >= SETTINGS_ADDR + SETTINGS_LEN)
    {
        addr = SETTINGS_ADDR;
    }
    else
    {
       addr += sizeof(settings_t);
    }

    return addr;
}

CCM_FUNC static uint32_t findPrevPageAddr(void)
{
    uint32_t addr = findCurrentPageAddr();

    /* Check if at first page */
    if(addr == SETTINGS_ADDR)
    {
        addr = SETTINGS_ADDR + SETTINGS_LEN;
    }
    else
    {
        addr -= sizeof(settings_t);
    }

    return addr;
}

CCM_FUNC static uint8_t writeFlash(int32_t addr, uint8_t *buffer, crc_t *crc, uint32_t len)
{
    // Check min/max length
    if (len > sizeof(settings_t) || len < 8)
        return 1;

    uint8_t i;
    *crc = getCrc(&crc32_config, (uint8_t*)buffer, len - 4); // Minus crc itself

    FLASH_Unlock();
    if (addr == SETTINGS_START) {
        FLASH_ErasePage(SETTINGS_START);
    }
    for (i = 0; i < len - 4; i += 2) {

      if (FLASH_ProgramHalfWord(addr, *((uint32_t*)(buffer + i))) != FLASH_COMPLETE)
      {
          FLASH_Lock();
          return 2;
      }
      addr += 2;
    }
    FLASH_ProgramWord(addr + 4, *crc);
    FLASH_Lock();
    return 0;
}

CCM_FUNC uint8_t readFlash(int32_t addr, uint8_t *buffer, crc_t *crc, uint32_t len)
{
    // Check min/max length
    if (len > sizeof(settings_t) || len < 8)
        return 1;

    *crc = getCrc(&crc32_config, (uint8_t *)addr, len - 4); // Minus crc itself
    memcpy(buffer, (uint8_t *)addr, len);

    return 0;
}

CCM_FUNC uint8_t readSettingsFromFlash(void)
{
    crc_t crc1 = 0;
    size_t len = sizeof(settings_t);

    if (readFlash(findCurrentPageAddr(), (uint8_t*)&settings_buf, &crc1, len) != 0)
        return 1;

    if (crc1 != settings_buf.crc)
    {
        // Try previous page
        if (readFlash(findPrevPageAddr(), (uint8_t*)&settings_buf, &crc1, len) != 0)
            return 2;
        if (crc1 != settings_buf.crc)
            return 3;
    }

    if (settings_buf.key != magic_key)
        return 4;

    memcpy(&settings, &settings_buf, sizeof(settings_buf));

    return 0;
}

CCM_FUNC uint8_t writeSettingsToFlash(void)
{
    crc_t crc1 = 0, crc2 = 0;
    uint8_t res1, res2;
    size_t len =  sizeof(settings_buf);
    memcpy(&settings_buf, &settings, sizeof(settings_buf));

    settings_buf.key = magic_key;
    settings_buf.cnt++;

    res1 = writeFlash(findNextPageAddr(), (uint8_t*)&settings_buf, &crc1, len);
    res2 = readFlash(findCurrentPageAddr(), (uint8_t*)&settings_buf, &crc2, len);

    // return 0 if write, read and crc comparison are successful
    if (res1 == 0 && res2 == 0 && crc1 == crc2)
        return 0;

    return 1;
}

CCM_FUNC uint8_t readVersionFromFlash(uint8_t idx, version_t* dst)
{
    const size_t len = sizeof(version_t);
    memcpy(dst, VERSION_PTR + (idx * len), len);

    return 0;
}

CCM_FUNC uint8_t writeVersionToFlash(uint8_t idx, const version_t* src)
{
    const size_t len = sizeof(version_t);
    uint32_t ver_buf;

    /* Copy to buffer */
    memcpy(&ver_buf, src + (idx * len), len);

    /* Unlock flash */
    FLASH_Unlock();

    /* Erase page and Write - We will loose settings */
    FLASH_ErasePage(SETTINGS_ADDR);
    FLASH_ProgramWord(VERSION_ADDR, ver_buf);

    /* Lock flash */
    FLASH_Lock();

    return 0;
}
