#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <stdint.h>

typedef enum {
  DS18B20_Resolution_9_bit    = 0x1F,
  DS18B20_Resolution_10_bit   = 0x3F,
  DS18B20_Resolution_11_bit   = 0x5F,
  DS18B20_Resolution_12_bit   = 0x7F
} DS18B20_Resolution;


void ds18b20_init(void);
float ds18b20_getTemperature(void);

#endif /* __DS18B20_H__ */
