#include "main.h"

#ifdef BLINK_WAY

#include "stm32f10x.h"

void _delay_us(uint32_t us)
{
	TIM3->PSC = 72-1; 						  //set prescaler
    TIM3->ARR = us; 						  //timer value
    TIM3->EGR |= TIM_EGR_UG; 				  //generate update event to write data in PSC and ARR
    TIM3->CR1 |= TIM_CR1_CEN | TIM_CR1_OPM;   //Start timer (bit CEN) and set single pass mode (bit OPM)
    
	while ((TIM3->CR1 & TIM_CR1_CEN) != 0);
}


int main()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructureC13;

	
	GPIO_InitStructureC13.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructureC13.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructureC13.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructureC13);
	
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);	
		_delay_us(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		_delay_us(1000);
	}
}

#endif
