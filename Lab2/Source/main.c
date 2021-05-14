#include "main.h"

int main()
{
	initGPIO();																 //Init all GPIOs
	initSystickTimer();														 //Init SysTick timer
	initTIM3(300);															 //initGPIO Timer3 with PWM 300 Hz
	while(1)
	{
		if(checkButtonClick(GPIOB->IDR & GPIO_IDR_IDR4))					 //checking PB4 button click
		{
			SysTickChangeStatus();											 //Enable or disable SysTick timer interrupts
			_delay_ms(1000);
		}
		else if(checkButtonClick(GPIOB->IDR & GPIO_IDR_IDR3))				 //checking PB3 button click
		{
			incrementTIM3DutyPercent();										 //inctement PWM duty (+1%)
			_delay_ms(200);
		}
	}
}

void initGPIO(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 									 //Alternate function IO clock enable
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_1;										 //010: JTAG-DP Disabled and SW-DP Enabled (Activate PB3)
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;										 //Enable GPIOA clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;										 //Enable GPIOB clock
	
	GPIOB->CRL &= ~GPIO_CRL_MODE4;											 //Input mode (sysTick enable button)
	GPIOB->CRL &= ~GPIO_CRL_CNF4;										     //Input with pull-up/pull-down (default)
	GPIOB->CRL |= GPIO_CRL_CNF4_1;											 //Input with pull-up/pull-down (default)
	GPIOB->BSRR |= GPIO_BSRR_BS4;											 //Pull up +3.3V
	
	GPIOB->CRL &= ~GPIO_CRL_MODE3;											 //Input mode (general Timer enable button)
	GPIOB->CRL &= ~GPIO_CRL_CNF3;											 //Input with pull-up/pull-down
	GPIOB->CRL |= GPIO_CRL_CNF3_1;											 //Input with pull-up/pull-down
	GPIOB->BSRR |= GPIO_BSRR_BS3;											 //Pull up +3.3V
	
	GPIOA->CRL |= (GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1);					 //50 MHz out mode (first signal)
	GPIOA->CRL &= ~GPIO_CRL_CNF2;											 //Push&Pull
	
	GPIOB->CRL |= (GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1);					 //50 MHz out mode (second signal)
	GPIOB->CRL &= ~GPIO_CRL_CNF0;											 //Alternate function Push&Pull
	GPIOB->CRL |= GPIO_CRL_CNF0_1;											 //Alternate function Push&Pull
}

void initSystickTimer(void)
{
	uint32_t period = (SystemCoreClock/1000)-1;								 //Timer period (1000 Hz)
	SysTick->LOAD = period;													 //Reload value
	SysTick->VAL = period;													 //Current value
	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);//Use processor clock source and enable SysTick exception
}

void SysTick_Handler (void)													 // Handlink systick interrupt, generating PWM
{
	static int tickCounter = 0;
	if(tickCounter == 5)
		GPIOA->BRR |= GPIO_BRR_BR2;											 // Reset PA2 to 0
	else if(tickCounter == 0)
		GPIOA->BSRR |= GPIO_BSRR_BS2;										 // Set PA2 to 1
	
	if(tickCounter == 9)
		tickCounter = 0;
	else
		tickCounter++;
}

void initTIM3(uint32_t freq)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;										 //Enable TIM3 clock
	
	TIM3->CCMR2 |= (TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2); //Set second PWM mode for 3th channel
	TIM3->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3P;							 //Set polarity for 3th channel
	
	TIM3->PSC = 31;															 //Prescaler = 32
	TIM3->ARR = (SystemCoreClock/freq/(TIM3->PSC+1))-1;						 //PWM period
	TIM3->CCR3 = ((TIM3->ARR+1)/100)-1;										 //PWM initial duty

	TIM3->CR1 |= TIM_CR1_CEN;												 //Enable timer
}

void SysTickChangeStatus(void)
{
	SysTick->CTRL ^= SysTick_CTRL_ENABLE_Msk;								 //Enable or disable SysTick timer interrupts
}

void incrementTIM3DutyPercent(void)									 		 //Cyclic inctementing PWM duty (+1%; 1-70%)
{
	if(TIM3->CCR3 == (((TIM3->ARR+1)/100*70)-1))
		TIM3->CCR3 = ((TIM3->ARR+1)/100)-1;
	else
		TIM3->CCR3 += ((TIM3->ARR+1)/100);
}

char checkButtonClick(uint32_t pin)											//Function for skipping bouncing clicks
{
	if(!pin)
	{
		_delay_ms(20);
		if(!pin)
			return TRUE;
	}
	
	return FALSE;
}

void _delay_ms(uint32_t ms)													//Simple cycle delay
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}
