#include "main.h"

#ifdef SECOND_WAY

#include "stm32f10x.h"

void _delay_ms(uint32_t ms)
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}

void PWM(float dutyCyclePercent1, float dutyCyclePercent2, int period)
{
	int GPIOAWorkTime = period/(100/dutyCyclePercent1);						//Calculate first delay
	int GPIOBWorkTime = period/(100/dutyCyclePercent2);						//Calculate second delay
	
	GPIOA->BSRR |= GPIO_BSRR_BS2;											//Set GPIOA 2 pin
	GPIOB->BSRR |= GPIO_BSRR_BS10;											//Set GPIOB 10 pin
	if(dutyCyclePercent1 > dutyCyclePercent2)
	{
		_delay_ms(GPIOBWorkTime);
		GPIOB->BRR |= GPIO_BRR_BR10;										//Reset GPIOB 10 pin
		_delay_ms(GPIOAWorkTime-GPIOBWorkTime);
		GPIOA->BRR |= GPIO_BRR_BR2;											//Reset GPIOA 2 pin
		_delay_ms(period-GPIOAWorkTime);
	}
	else
	{
		_delay_ms(GPIOAWorkTime);
		GPIOA->BRR |= GPIO_BRR_BR2;											//Reset GPIOA 2 pin
		_delay_ms(GPIOBWorkTime-GPIOAWorkTime);
		GPIOB->BRR |= GPIO_BRR_BR10;										//Reset GPIOB 10 pin
		_delay_ms(period-GPIOBWorkTime);
	}
}

int main()
{
	RCC->APB2ENR|= RCC_APB2ENR_IOPAEN;										//Enable GPIOA clock
	RCC->APB2ENR|= RCC_APB2ENR_IOPBEN;										//Enable GPIOB clock
	 
	GPIOA->CRL |= (GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1);					//50 MHz out mode
	GPIOA->CRL &= ~GPIO_CRL_CNF2;											//Push&Pull
	
	GPIOB->CRH |= (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1);					//50 MHz out mode
	GPIOB->CRH &= ~GPIO_CRH_CNF10;											//Push&Pull
	
	while(1)
	{
		PWM(50,20,10);														//50% GPIOA2, 20% GPIOB10, 10 ms period
	}
}

#endif
