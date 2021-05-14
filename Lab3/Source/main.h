#include "cmsis_os2.h"
#include "stm32f10x.h"

void gpioA_thread (void *argument);
void initGpioA(void);
void createThreads(void);

osThreadId_t thread1_0;
osThreadId_t thread1_1;
osThreadId_t thread1_2;
osThreadId_t thread1_3;

osThreadId_t thread2_0;
osThreadId_t thread2_1;
osThreadId_t thread2_2;
osThreadId_t thread2_3;
osThreadId_t thread2_4;

osThreadId_t thread3_0;
osThreadId_t thread3_1;
osThreadId_t thread3_2;

osThreadId_t thread4_0;
osThreadId_t thread4_1;
