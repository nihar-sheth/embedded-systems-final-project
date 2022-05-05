/*
COE718 Final Project

Name: Nihar Sheth
Student ID: 500745364

Course: COE718 - Embedded Systems Design
Instructor: Dr. Saber Amini

Section: 05
TA: Farnaz Sarhangian
Demo date & time: Friday, December 4, 2020, 9:30 AM
*/

//////////////////////////////////////////////////////////////////////////////////////
//									Library and Header Declarations		      												//
//////////////////////////////////////////////////////////////////////////////////////	

#define osObjectsPublic                 // Define objects in main module
#define PI 3.141592654

#include <stdio.h>
#include <math.h>
#include <ctype.h>                    
#include <string.h>                   
#include <stdbool.h>

#include "LPC17xx.h"                    // Device header
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_ADC.h"                  // ::Board Support:A/D Converter
#include "Board_Joystick.h"							// ::Board Support Joystick

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "osObjects.h"                  // RTOS object definitions

//////////////////////////////////////////////////////////////////////////////////////
//										Variable and Function Declarations														//
//////////////////////////////////////////////////////////////////////////////////////	

void delay () { 
	long x;
	long max = 0;
	
	for (x = 0; x < 100000; x++) {
		max++;
	}
}

/*Part 2 Assignment Declarations*/

//	Mutex (mut)
osMutexId t_mut;
osMutexDef(t_mut);

void TaskA_mut (void const *argument);
void TaskB_mut (void const *argument);
void TaskC_mut (void const *argument);

//		Mutex thread priorities
osThreadDef(TaskA_mut, osPriorityHigh, 1, 0);
osThreadDef(TaskB_mut, osPriorityNormal, 1, 0);
osThreadDef(TaskC_mut, osPriorityBelowNormal, 1, 0);

//		Mutex thread IDs
osThreadId t_main_mut;
osThreadId t_TaskA_mut;
osThreadId t_TaskB_mut;
osThreadId t_TaskC_mut;

/*********************/

//	Semaphore (sem)
osSemaphoreId t_sem;
osSemaphoreDef(t_sem);

void TaskA_sem (void const *argument);
void TaskB_sem (void const *argument);
void TaskC_sem (void const *argument);

//		Semaphore thread priorities
osThreadDef(TaskA_sem, osPriorityHigh, 1, 0);
osThreadDef(TaskB_sem, osPriorityNormal, 1, 0);
osThreadDef(TaskC_sem, osPriorityBelowNormal, 1, 0);

//		Sempahore thread IDs
osThreadId t_main_sem;
osThreadId t_TaskA_sem;
osThreadId t_TaskB_sem;
osThreadId t_TaskC_sem;

/*********************/

//	Resemble semaphore (rsm)
unsigned long rsm;
int rsm_A;

void TaskA_rsm (void const *argument);
void TaskB_rsm (void const *argument);
void TaskC_rsm (void const *argument);

//		Resemble semaphore thread priorities
osThreadDef(TaskA_rsm, osPriorityHigh, 1, 0);
osThreadDef(TaskB_rsm, osPriorityNormal, 1, 0);
osThreadDef(TaskC_rsm, osPriorityBelowNormal, 1, 0);

//		Resemble semaphore thread IDs
osThreadId t_main_rsm;
osThreadId t_TaskA_rsm;
osThreadId t_TaskB_rsm;
osThreadId t_TaskC_rsm;

//		Resemble semaphore functions
void semWait () {
	long k;
	
	for (k = 0; k < 150000; k++) {
		rsm--;
	}
	
	rsm_A = 0; // Unavailable
}

void semRelease (){
	long k;
	for (k = 0; k < 150000; k++){
		rsm++;
	}
	rsm_A = 1; // Available
}

/*********************/

//	Resources 
void Resource_1 () { 
	long x;
	long use_count_1 = 0;
	
	for (x = 0; x < 150000; x++) {
		use_count_1++;
	}
}

void Resource_2 () { 
	long x;
	long use_count_2 = 0;
	
	for (x = 0; x < 150000; x++){
		use_count_2++;
	}
}

/*********************/

//	Mutex threads
void TaskA_mut (void const *argument) {
	for (;;) {
		delay(); 
		osThreadSetPriority(t_TaskC_mut, osPriorityHigh); // Solution uncomment
		
		osMutexWait(t_mut, osWaitForever);
		Resource_1();
		osMutexRelease(t_mut);
		osThreadSetPriority(t_TaskC_mut, osPriorityBelowNormal); // Solution uncomment
	}
}

void TaskB_mut (void const *argument) {
	for (;;) {
		//
	}
}

void TaskC_mut (void const *argument) {
	for (;;) {
		osMutexWait(t_mut, osWaitForever);
		Resource_1();
		osMutexRelease(t_mut);
	}
}

/*********************/

//	Semaphore threads
void TaskA_sem (void const *argument) {
	for (;;) {
		delay(); 
		osThreadSetPriority(t_TaskC_sem, osPriorityHigh); // Solution uncomment
	
		osSemaphoreWait(t_sem, osWaitForever);
		Resource_2();
		osSemaphoreRelease(t_sem);
		osThreadSetPriority(t_TaskC_sem, osPriorityBelowNormal); // Solution uncomment
	}
}

void TaskB_sem (void const *argument) {
	for (;;) {
		//
	}
}

void TaskC_sem (void const *argument) {
	for (;;) {
		osSemaphoreWait(t_sem, osWaitForever);
		Resource_2();
		osSemaphoreRelease(t_sem);
	}
}


/*********************/

//	Resemble semaphore threads
void TaskA_rsm (void const *argument) {
	for (;;) {
		delay(); 
		osThreadSetPriority(t_TaskC_rsm, osPriorityHigh); // Solution uncomment
		
		if (!rsm_A)
			osSignalWait(0x02,osWaitForever);
		
		semWait();
		osSignalSet(t_TaskC_rsm,0x01);						
		osSignalWait(0x02,osWaitForever);
		osThreadSetPriority(t_TaskC_rsm, osPriorityBelowNormal); // Solution uncomment
	}
}

void TaskB_rsm (void const *argument) {
	for (;;) {
		//
	}
}

void TaskC_rsm (void const *argument) {
	for (;;) {
		semRelease();
		osSignalSet(t_TaskA_rsm, 0x02);
		osSignalWait(0x01,osWaitForever);
	}
}

/*********************/

/*Part 3 Assignment Declarations*/

double count_Thread_1 = 0;
double count_Thread_2 = 0;
double count_Thread_3 = 0;

osStatus stat[5];
osThreadId t_Thread_1;
osThreadId t_Thread_2;
osThreadId t_Thread_3;

//	Joinable threads
void Thread_1 (void const *arg) {
	int x;
	LED_On(0);
	osDelay(1000);
	
	for (x = 0; x <= 200; x++) {
		count_Thread_1 += 2*x + 2.0;
	}
	
	int n;
	LED_Off(0);
	osDelay(1000);
	
  osThreadTerminate(t_Thread_1);
}

void Thread_2 (void const *arg) {
	 int n;
	 LED_On(1);
	 osDelay(1000);
	 
	 for(n = 1; n <= 16; n++) {
		 count_Thread_2 += (n + 1.00)/n;
	 }
	 
	 LED_Off(1);
	 osDelay(1000);
	 
	 osThreadTerminate(t_Thread_2);
}

void Thread_3 (void const *arg) {
	 double r = 1;
	 LED_On(2);
	 osDelay(1000);
	 
	 count_Thread_3 = 45*PI*(r*r);
	 
	 LED_Off(2);
	 osDelay(1000);
	 
	 osThreadTerminate(t_Thread_3);
}

//////////////////////////////////////////////////////////////////////////////////////
//															MAIN FUNCTION																				//
//////////////////////////////////////////////////////////////////////////////////////	

int main (void) {
	
	//	Joystick variables
	uint32_t select = 0;
	
	//	System and pin initialization
	SystemInit();
  LED_Initialize(); // LED Initialization
  ADC_Initialize(); // ADC Initialization
	Joystick_Initialize(); // Joy Stick Initialization
	
	LPC_SC->PCONP |= (1 << 15); // Power up bit 15 for joystick
    
    /* P1.20, P1.23..26 is GPIO (Joystick) */
    LPC_PINCON->PINSEL3 &= ~((3<< 8)|(3<<14)|(3<<16)|(3<<18)|(3<<20));
    
    /* P1.20, P1.23..26 is input */
    LPC_GPIO1->FIODIR &= ~((1<<20)|(1<<23)|(1<<24)|(1<<25)|(1<<26));
	
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100); // Generate interrupt each 10 ms
  osKernelInitialize (); // Initialize CMSIS-RTOS
	
	osThreadDef (Thread_1, osPriorityHigh, 1, 0); 
  osThreadDef (Thread_2, osPriorityAboveNormal, 1, 0);
  osThreadDef (Thread_3, osPriorityNormal, 1, 0);
	
	//	Selection print statements
	printf("\n**MAKE SELECTION ON GPIO1**\n");
	printf("Mutex mode - Pin 24 (right joystick)\n");
	printf("Semaphore mode - Pin 25 (down joystick)\n");
	printf("Resemble semaphore mode - Pin 26 (left joystick)\n");
	printf("Joinable threads mode - Pin 23 (up joystick)\n");
	
	//	Read input and perform selected mode
	while (1) {
		select = Joystick_GetState();
		
		switch(select) {
			
			// P24 for mutex mode
			case 0x00000002:
				printf("\t> Mutex mode\n");
			
			t_mut = osMutexCreate(osMutex(t_mut));
			t_main_mut = osThreadGetId();
			osThreadSetPriority(t_main_mut,osPriorityHigh);
			
			t_TaskC_mut = osThreadCreate(osThread(TaskC_mut), NULL);
			osDelay(50);
			
			t_TaskB_mut = osThreadCreate(osThread(TaskB_mut), NULL);
			osDelay(50);
			
			t_TaskA_mut = osThreadCreate(osThread(TaskA_mut), NULL);
			
			osThreadTerminate(t_main_mut);
			break;
			
			// P25 for semaphore mode
			case 0x00000010:
				printf("\t> Semaphore mode\n");
			
			t_sem = osSemaphoreCreate(osSemaphore(t_sem),1);
			t_main_sem = osThreadGetId();
			osThreadSetPriority(t_main_sem,osPriorityHigh);
			
			t_TaskC_sem = osThreadCreate(osThread(TaskC_sem), NULL);
			osDelay(50);
			
			t_TaskB_sem = osThreadCreate(osThread(TaskB_sem), NULL);
			osDelay(50);
			
			t_TaskA_sem = osThreadCreate(osThread(TaskA_sem), NULL);
			
			osThreadTerminate(t_main_sem);
			break;
			
			// P26 for resemble semaphore mode
			case 0x00000001:
				printf("\t> Resemble semaphore mode\n");
			
			t_main_rsm = osThreadGetId();
			osThreadSetPriority(t_main_rsm,osPriorityHigh);
			
			t_TaskC_rsm = osThreadCreate(osThread(TaskC_rsm), NULL);
			osDelay(50);
			
			t_TaskB_rsm = osThreadCreate(osThread(TaskB_rsm), NULL);
			osDelay(50);
			
			t_TaskA_rsm = osThreadCreate(osThread(TaskA_rsm), NULL);
			
			osThreadTerminate(t_main_rsm);
			break;
			
			// P23 for joinable threads mode
			case 0x00000008:
				printf("\t> Joinable threads mode\n");
			
			osDelay(100);
			t_Thread_1 = osThreadCreate (osThread(Thread_1), NULL); 
      osDelay(100);
			
			t_Thread_2 = osThreadCreate (osThread(Thread_2), NULL);
			osDelay(100);
			
			t_Thread_3 = osThreadCreate (osThread(Thread_3), NULL);
			break;
		}
  }
	
  osKernelStart (); // Start thread execution 
}
