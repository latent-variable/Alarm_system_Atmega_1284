/*  My Name & SID:Lino Gonzalez Valdvinos, lgonz041@ucr.edu, 861300001
*	Lab Section: 25
*	Assignment: Final Project
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "usart.h"
#include "EEPROM.c"
#include "Timer.c"
#include "io.c"
#include "keypad.h"
#include <stdio.h>
#include <stdlib.h>

//global Variables 
unsigned char Alarm_Status;		//0x00 alarm is off 0x01 alarm is on
unsigned char passcode[5];		//4 number passcode
unsigned char key;				// input from the keypad
unsigned char Temp_choice;		//
unsigned char Detected;			//Alarm has been Triggered
unsigned char Beep_on;	
unsigned char BT;
unsigned char Temp;


//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;


////////////State machine code////////////
#include "HC-05.c"
#include "Thermistor.c"
#include "PIR.c"
#include "key.c"
#include "Alarm.c"
#include "Sound.c"
// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	// Buttons PORTB[0-7], set AVR PORTA to pull down logic
	DDRB = 0xF0; PORTB = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRA = 0x00; PORTA = 0xFF;	//input from PIR and Temp Sensor
	//Declare an array of tasks 
	static task  task1, task2, task3, task4, task5, task6;
	task *tasks[] = {  &task1, &task2, &task3, &task4, &task5, &task6};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	
	// Task 1
	task1.state = 0;//Task initial state.
	task1.period = 200;//Task Period.
	task1.elapsedTime = 1;//Task current elapsed time.
	task1.TickFct = &Alarm_tick;//Function pointer for the tick.
	
	// Task 2
	task2.state = 0;//Task initial state.
	task2.period = 100;//Task Period.
	task2.elapsedTime = 1;//Task current elapsed time.
	task2.TickFct = &keypad;//Function pointer for the tick.
	
	// Task 3
	task3.state = 0;//Task initial state.
	task3.period = 200;//Task Period.
	task3.elapsedTime = 1;//Task current elapsed time.
	task3.TickFct = &PIR_Tick;//Function pointer for the tick.
	
	// Task 4
	task4.state = 0;//Task initial state.
	task4.period = 2;//Task Period.
	task4.elapsedTime = 1;//Task current elapsed time.
	task4.TickFct = &Sound_Tick;//Function pointer for the tick.
	
	// Task 5
	task5.state = 0;//Task initial state.
	task5.period = 1;//Task Period.
	task5.elapsedTime = 1;//Task current elapsed time.
	task5.TickFct = &Beep_tick;//Function pointer for the tick.
	
	// Task 6
	task6.state = 0;//Task initial state.
	task6.period = 200;//Task Period.
	task6.elapsedTime = 1;//Task current elapsed time.
	task6.TickFct = &BT_tick;//Function pointer for the tick.

	/* Task 7
	task7.state = 0;//Task initial state.
	task7.period = 500;//Task Period.
	task7.elapsedTime = 1;//Task current elapsed time.
	task7.TickFct = &Temp_tick;//Function pointer for the tick.
	*/
	// Set the timer and turn it on
	TimerSet(1);
	TimerOn();
	LCD_init();
	initUSART();
	ADC_init();
	
	//set_PWM(.9);
	//initialize eventually in memory
	Alarm_Status = 0x00;
	Temp_choice = 'F';
	

	//Default passcode if not already saved in memory
	if (EEPROM_read(1999) == EEPROM_read(2000)){
		EEPROM_write(2000,'4');
		EEPROM_write(2001,'3');
		EEPROM_write(2002,'2');
		EEPROM_write(2003,'1');
	}
	
	//read from memory the code
	passcode[0] = EEPROM_read(2000);
	passcode[1] = EEPROM_read(2001);
	passcode[2] = EEPROM_read(2002);
	passcode[3] = EEPROM_read(2003);
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}
