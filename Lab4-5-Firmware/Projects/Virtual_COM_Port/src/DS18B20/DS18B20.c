#include "DS18B20.h"

#include <ow_driver.h>
#include <onewire.h>
#include <ow_driver_stm32f10x_gpio.h>
#include <dallas.h>
#include "delay.h"

#define MAX_OW_DEV_COUNT 1

uint8_t scratch_pad[__SCR_LENGTH];
uint8_t dev_count;
uint8_t dev_addr[MAX_OW_DEV_COUNT][8];

ow_driver_ptr driver;
owu_struct_t o2;

void ds18b20_init(void) {
	// Init OneWire GPIO driver on GPIOA and pin 5 (PA5)
	init_driver(&driver, E_GPIOA+5);
	owu_init(&o2, driver);

	dev_count = 0;
	while(owu_search(&o2, dev_addr[dev_count])) {
		dev_count++;
		if (dev_count >= MAX_OW_DEV_COUNT) {
			break;
		}
	}
}

float ds18b20_getTemperature(void) {
	ds_convert_all(&o2);
	
	delay_us(4000);
	
	ds_read_scratchpad(&o2, dev_addr[0], scratch_pad);
	
	return ds_get_temp_c(scratch_pad);
}
