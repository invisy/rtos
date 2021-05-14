#include "stm32f10x.h"

#define TRUE 1
#define FALSE 0

void _delay_ms(uint32_t ms);
char checkButtonClick(uint32_t pin);
void initGPIO(void);
void initSystickTimer(void);
void initTIM3(uint32_t freq);
void SysTickChangeStatus(void);
void incrementTIM3DutyPercent(void);
void SysTick_Handler (void);
