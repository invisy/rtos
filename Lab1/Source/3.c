#include "main.h"

#ifdef THIRD_WAY

#include "stm32f10x.h"

void _delay_ms(uint32_t ms)
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}

void PWM(float dutyCyclePercent1, float dutyCyclePercent2, int period)
{
	int GPIOAWorkTime = period/(100/dutyCyclePercent1);
	int GPIOBWorkTime = period/(100/dutyCyclePercent2);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);											//Set GPIOA 2 pin
	GPIO_SetBits(GPIOB, GPIO_Pin_10);											//Set GPIOB 10 pin
	if(dutyCyclePercent1 > dutyCyclePercent2)
	{
		_delay_ms(GPIOBWorkTime);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);										//Reset GPIOB 10 pin
		_delay_ms(GPIOAWorkTime-GPIOBWorkTime);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);										//Reset GPIOA 2 pin
		_delay_ms(period-GPIOAWorkTime);
	}
	else
	{
		_delay_ms(GPIOAWorkTime);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);										//Reset GPIOA 2 pin
		_delay_ms(GPIOBWorkTime-GPIOAWorkTime);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);										//Reset GPIOB 10 pin
		_delay_ms(period-GPIOBWorkTime);
	}
}

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructureA2;										//Init structure
	GPIO_InitTypeDef  GPIO_InitStructureB10;									//Init second structure

	
	GPIO_InitStructureA2.GPIO_Pin = GPIO_Pin_2;									//GPIOA pin2
	GPIO_InitStructureA2.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA2.GPIO_Speed = GPIO_Speed_50MHz;							//Set structure field GPIO_Pin as GPIO_Pin_2
	
	GPIO_InitStructureB10.GPIO_Pin = GPIO_Pin_10;								//GPIOB pin10
	GPIO_InitStructureB10.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureB10.GPIO_Speed = GPIO_Speed_50MHz;						//50 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructureA2);									//Init GPIOA Pin2
	GPIO_Init(GPIOB, &GPIO_InitStructureB10);									//Init GPIOB Pin10
	
	while(1)
	{
		PWM(50,20,10);															//50% GPIOA2, 20% GPIOB10, 10 ms period
	}
}

#endif
