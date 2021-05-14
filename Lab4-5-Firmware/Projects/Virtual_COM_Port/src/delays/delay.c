#include "delay.h"

void init_delay(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void delay_us(uint32_t us)
{
	TIM3->PSC = 72-1; 						//set prescaler
    TIM3->ARR = us; 						//timer value
    TIM3->EGR |= TIM_EGR_UG; 				//generate update event to write data in PSC and ARR
    TIM3->CR1 |= TIM_CR1_CEN | TIM_CR1_OPM;   //Start timer (bit CEN) and set single mode (bit OPM)
    
	while ((TIM3->CR1 & TIM_CR1_CEN) != 0);
}
