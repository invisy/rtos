#include "main.h"

void gpioA_thread (void *argument)						//Our thread function with parameters
{
	while(1) {
		GPIO_SetBits(GPIOA, (*(uint16_t*)argument));	//Set pin to logical 1
	}
}

void initGpioA(void)
{
	GPIO_InitTypeDef Config_GPIO_A;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_StructInit(&Config_GPIO_A);											//Initialize structure
	Config_GPIO_A.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	//Configure pins
	Config_GPIO_A.GPIO_Mode = GPIO_Mode_Out_PP;									//Configure mode (push&pull)
	Config_GPIO_A.GPIO_Speed = GPIO_Speed_50MHz;								//Configure port speed
	GPIO_Init(GPIOA, &Config_GPIO_A);											//Initialize GPIO from structure
}

void createThreads(void)
{
	uint16_t gpioa_pin0 = GPIO_Pin_0;	//GPIOA pin mask
	uint16_t gpioa_pin1 = GPIO_Pin_1;
	uint16_t gpioa_pin2 = GPIO_Pin_2;
	uint16_t gpioa_pin3 = GPIO_Pin_3;
	
	thread1_0 = osThreadNew(gpioA_thread, &gpioa_pin0, NULL);		//creating instance of thread
	thread1_1 = osThreadNew(gpioA_thread, &gpioa_pin0, NULL);		
	thread1_2 = osThreadNew(gpioA_thread, &gpioa_pin0, NULL);
	thread1_3 = osThreadNew(gpioA_thread, &gpioa_pin0, NULL);
	
	thread2_0 = osThreadNew(gpioA_thread, &gpioa_pin1, NULL);
	thread2_1 = osThreadNew(gpioA_thread, &gpioa_pin1, NULL);
	thread2_2 = osThreadNew(gpioA_thread, &gpioa_pin1, NULL);
	thread2_3 = osThreadNew(gpioA_thread, &gpioa_pin1, NULL);
	thread2_4 = osThreadNew(gpioA_thread, &gpioa_pin1, NULL);
	
	thread3_0 = osThreadNew(gpioA_thread, &gpioa_pin2, NULL);
	thread3_1 = osThreadNew(gpioA_thread, &gpioa_pin2, NULL);
	thread3_2 = osThreadNew(gpioA_thread, &gpioa_pin2, NULL);
	
	thread4_0 = osThreadNew(gpioA_thread, &gpioa_pin3, NULL);
	thread4_1 = osThreadNew(gpioA_thread, &gpioa_pin3, NULL);
}

int main (void)
{
	initGpioA();					// initialize port GPIOA
	osKernelInitialize(); 			// initialize CMSIS-RTOS
	createThreads();				// create all threads
	osKernelStart(); 				// start thread execution
	
	while(1) {}
}
