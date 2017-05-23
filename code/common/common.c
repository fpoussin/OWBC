
#include "common.h"
#include "string.h"
#include "vectors.h"
#include <stdlib.h>

inline uint32_t getuuid32(void) {

    return MCU_UUID[0] * MCU_UUID[1] * MCU_UUID[2];
}

uint16_t rand16(uint16_t min, uint16_t max)
{
    uint16_t r;
    const uint16_t range = 1 + max - min;
    const uint16_t buckets = (RAND_MAX & 0xFFFF) / range;
    const uint16_t limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = (rand() & 0xFFFF);
    } while (r >= limit);

    return min + (r / buckets);
}

uint32_t rand32(uint32_t min, uint32_t max)
{
    uint32_t r;
    const uint32_t range = 1 + max - min;
    const uint32_t buckets = RAND_MAX / range;
    const uint32_t limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

inline uint32_t leToUInt32(uint8_t *ptr) {

  return ((uint32_t)ptr[3] << 24) |
      ((uint32_t)ptr[2] << 16) |
      ((uint32_t)ptr[1] << 8) |
      (uint32_t)ptr[0];
}

inline uint16_t leToUInt16(uint8_t *ptr) {

  return ((uint16_t)ptr[1] << 8) | (uint16_t)ptr[0];
}

inline uint32_t beToUInt32(uint8_t *ptr) {

  return ((uint32_t)ptr[0] << 24) |
      ((uint32_t)ptr[1] << 16) |
      ((uint32_t)ptr[2] << 8) |
      (uint32_t)ptr[3];
}

inline uint16_t beToUInt16(uint8_t *ptr) {

  return ((uint16_t)ptr[0] << 8) | (uint16_t)ptr[1];
}

uint8_t checksum(const uint8_t *data, uint8_t length)
{
    uint8_t i;
    uint8_t sum = 0;

    for (i = 0; i < length; i++)
      sum += data[i];

    return sum;
}

inline bool getSwitch1(void)
{
    return palReadPad(PORT_BUTTON1, PAD_BUTTON1) == PAL_LOW;
}

inline int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

