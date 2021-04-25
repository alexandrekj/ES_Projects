#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

osThreadId_t thread1_id, thread2_id, thread3_id;
osMutexId_t mutex1_id, mutex2_id;

typedef struct structLED{
  uint8_t LED;
  uint32_t freq;
} structLED;


void thread1(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    osMutexAcquire(mutex1_id, osWaitForever); // Adquire mutex 1 
    tick = osKernelGetTickCount();      
    osMutexAcquire(mutex2_id, osWaitForever); // Adquire mutex 2
    state ^= LED1;    
    LEDWrite(LED1, state);
    osMutexRelease(mutex2_id);                // Libera mutex 2
    osDelayUntil(tick + 100);
    osMutexRelease(mutex1_id);                  //Libera mutex 1
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    osMutexAcquire(mutex1_id, osWaitForever); // Adquire mutex 2 
    tick = osKernelGetTickCount();
    state ^= LED2;
    osMutexAcquire(mutex2_id, osWaitForever); // Adquire mutex 1 
    LEDWrite(LED2, state);
    osMutexRelease(mutex2_id);                  //Libera mutex 1
    osDelayUntil(tick + 100);
    osMutexRelease(mutex1_id);                  //Libera mutex 2
  } // while
} // thread2

void thread3(void *arg){
  uint8_t state = 0;
  structLED* LEDn = (structLED*) arg;
  
  uint32_t tick = LEDn->freq;
  uint32_t LEDx = LEDn->LED;
  
  while(1){
    
    state ^= LEDx;
    LEDWrite(LEDx, state);
    
    osDelay(tick);
  } // while
} // thread3

  void main(void){
  LEDInit(LED2 | LED1 | LED3 | LED4);

  osKernelInitialize();

  mutex1_id = osMutexNew(NULL);
  mutex2_id = osMutexNew(NULL);
  
  thread1_id = osThreadNew(thread1, NULL, NULL);
  thread2_id = osThreadNew(thread2, NULL, NULL);
  
  
  
  structLED LED1_struct,LED2_struct,LED3_struct,LED4_struct;
  
  LED1_struct.LED = LED1;
  LED1_struct.freq = 200;
  LED2_struct.LED = LED2;
  LED2_struct.freq = 300;
  LED3_struct.LED = LED3;
  LED3_struct.freq = 500;
  LED4_struct.LED = LED4;
  LED4_struct.freq = 700; 
  
  
//  thread3_id = osThreadNew(thread3, &LED1_struct, NULL);
//  thread3_id = osThreadNew(thread3, &LED2_struct, NULL);
//  thread3_id = osThreadNew(thread3, &LED3_struct, NULL);
//  thread3_id = osThreadNew(thread3, &LED4_struct, NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
