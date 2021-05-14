#include "main.h"

#ifdef FIRST_WAY

#include "stm32f10x.h"

uint32_t* RCC_APB2ENR;
uint32_t* GPIO_A;
uint32_t* GPIO_A_CRL;
uint32_t* GPIO_A_BSRR;
uint32_t* GPIO_A_BRR;
uint32_t* GPIO_B;
uint32_t* GPIO_B_CRH;
uint32_t* GPIO_B_BSRR;
uint32_t* GPIO_B_BRR;

void _delay_ms(uint32_t ms)										//simple nop delay
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}

void PWM(float dutyCyclePercent1, float dutyCyclePercent2, int period)
{
	int GPIOAWorkTime = period/(100/dutyCyclePercent1); 		
	int GPIOBWorkTime = period/(100/dutyCyclePercent2); 		
	*GPIO_A_BSRR |= (1 << 2);									//Set 2nd GPIOA pin (2nd bit) to 1
	*GPIO_B_BSRR |= (1 << 10);									//Set 10th GPIOB pin (10nd bit) to 1
	if(dutyCyclePercent1 > dutyCyclePercent2)
	{
		_delay_ms(GPIOBWorkTime);
		*GPIO_B_BRR |= (1 << 10);								//Reset 10th GPIOB pin (10nd bit) to 0
		_delay_ms(GPIOAWorkTime-GPIOBWorkTime);
		*GPIO_A_BRR |= (1 << 2);								//Reset 2nd GPIOA pin (2th bit) to 0
		_delay_ms(period-GPIOAWorkTime);
	}
	else
	{
		_delay_ms(GPIOAWorkTime);
		*GPIO_A_BRR |= (1 << 2);								//Reset 2nd GPIOA pin (2th bit) to 0
		_delay_ms(GPIOBWorkTime-GPIOAWorkTime);
		*GPIO_B_BRR |= (1 << 10);								//Reset 10th GPIOB pin (10nd bit) to 0
		_delay_ms(period-GPIOBWorkTime);
	}
}

int main()
{
	RCC_APB2ENR = (uint32_t*)(0x40021000+0x18); 				//RCC region base address + APB2ENR offset
	GPIO_A = (uint32_t*)0x40010800;								//GPIOA base address
	GPIO_A_CRL = GPIO_A;										//GPIOA base address; CRL offset = 0x0
	GPIO_A_BSRR = (uint32_t*)((char*)GPIO_A+0x10);				//GPIOA base address + BSRR offset
	GPIO_A_BRR = (uint32_t*)((char*)GPIO_A+0x14);				//GPIOA base address + BRR offset
	GPIO_B = (uint32_t*)0x40010C00;								//GPIOB base address
	GPIO_B_CRH = (uint32_t*)((char*)GPIO_B+0x4);				//GPIOB base address + CRH offset
	GPIO_B_BSRR = (uint32_t*)((char*)GPIO_B+0x10);				//GPIOB base address + BSRR offset
	GPIO_B_BRR = (uint32_t*)((char*)GPIO_B+0x14);				//GPIOB base address + BRR offset
	
	*RCC_APB2ENR |= (1 << 2 | 1 << 3); 							//Set RCC_APB2ENR_IOPAEN (2nd bit) and RCC_APB2ENR_IOPBEN (3rd bit) to 1
	 
	*GPIO_A_CRL |= (1 << 8 | 1 << 9);  							//Set MODE2_0 (8th bit) and MODE2_1 (9rd bit) to 1
	*GPIO_A_CRL &= ~(1 << 10 | 1 << 11); 						/*Set СNF2_0 (10th bit) and CNF2_1 (11th bit) to 0 
																(actually we can reset only 10th bit, 11th is set to 0 by default)*/
	
	*GPIO_B_CRH |= (1 << 8 | 1 << 9);  							//Set MODE10_0 (8th bit) and MODE10_1 (9rd bit) to 1
	*GPIO_B_CRH &= ~(1 << 10 | 1 << 11);						//Set СNF10_0 (10th bit) and CNF10_1 (11th bit) to 0 (actually we can reset only 10th bit, 11th is set to 0 by default)
	
	while(1)													//Polling
	{
		PWM(50,20,10);											//50% GPIOA2, 20% GPIOB10, 10 ms period
	}
}

#endif
