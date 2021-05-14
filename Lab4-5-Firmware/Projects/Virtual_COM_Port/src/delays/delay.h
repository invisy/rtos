#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdint.h>
#include "stm32f10x.h"

void init_delay(void);
void delay_us(uint32_t us);

#endif
