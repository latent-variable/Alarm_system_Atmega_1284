/*  My Name & SID:Lino Gonzalez Valdvinos, lgonz041@ucr.edu, 861300001
*	Lab Section: 25
*	Assignment: Finnal Project
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.c"
#include "io.c"
#include "PWM.c"
#include "keypad.h"
#include <stdio.h>
#include <stdlib.h>

//--------End find GCD function ----------------------------------------------
//global Variables 
unsigned char Alarm_Status; 
unsigned char passcode[5];
unsigned char key;
unsigned char Temp_choice;


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

enum Keypad_States{wait_input, input_press, input }key_state;
int keypad(){
	unsigned char x;
	x = GetKeypadKey();
	switch(key_state){
			case wait_input:
				if( x  == '\0' ){
					key_state = wait_input;
					key = 0xFF;
				}else{
					key_state = input_press;
					key = x;
				}
				break;
			case input_press:
				if(x == '\0'){
					key_state = input;
				}else{
					key_state = input_press;
				}
				break;
			case input:
				key_state = wait_input;
				//Display_tick();
				break;
	}
}


enum DisplayState{ dstate} display_state;
int  Display_tick(){
		unsigned char y;
		
		switch (display_state){
			case dstate:
			y = GetKeypadKey();
			switch (y) {
				case '\0': break; 
				case '1': LCD_WriteData('1');  break; // hex equivalent
				case '2': LCD_WriteData('2');  break;
				case '3': LCD_WriteData('3');  break;
				case '4': LCD_WriteData('4');  break;
				case '5': LCD_WriteData('5');  break;
				case '6': LCD_WriteData('6');  break;
				case '7': LCD_WriteData('7');  break;
				case '8': LCD_WriteData('8');  break;
				case '9': LCD_WriteData('9');  break;
				case 'A': LCD_WriteData('A');  break;
				case 'B': LCD_WriteData('B');  break;
				case 'C': LCD_WriteData('C');  break;
				case 'D': LCD_WriteData('D');  break;
				case '*': LCD_WriteData('*');  break;
				case '0': LCD_WriteData('0');  break;
				case '#': LCD_WriteData('#');  break;
				case 0xFF: break;
				default:    break; // Should never occur. Middle LED off.
				break;
			}
		
		}
		return 0;
}


unsigned char Room_temp(){
	return 0x28;
}

enum AlarmStates{Alarm_menu_display, Alarm_menu,Alarm_on,Alarm_turn_off, Alarm_off,Alarm_settings_display, Alarm_settings, temp_settings, passcode_settings,code1,code2,code3,code4, Acode1,Acode2,Acode3,Acode4,Rcode1,Rcode2,Rcode3,Rcode4, }alarm_state;
	
int Alarm_tick(){
	unsigned char selection = key;
	unsigned char temperature = 82;
	char mystr[5];
	switch(alarm_state){
		case Alarm_menu_display:
			sprintf( mystr, "%d", temperature);
			if (Alarm_Status == 0){
				LCD_DisplayString(1,"Alarm Off       A:ARM B:Settings"  );
				LCD_Cursor(12);
				if (Temp_choice == 'F') LCD_WriteData('F');
				if (Temp_choice == 'C') LCD_WriteData('C');
				LCD_Cursor(13);
				LCD_WriteData(mystr[0]);
				LCD_Cursor(14);
				if ( temperature > 0x09) LCD_WriteData(mystr[1]);
				LCD_Cursor(15);
				if( temperature > 0x63) LCD_WriteData(mystr[2]);
				LCD_Cursor(33);
			}
			else{
				LCD_DisplayString(1,"Alarm On        A:ARM B:Settings"  );
				LCD_Cursor(12);
				if (Temp_choice == 'F') LCD_WriteData('F');
				if (Temp_choice == 'C') LCD_WriteData('C');
				LCD_Cursor(13);
				LCD_WriteData(mystr[0]);
				LCD_Cursor(14);
				if ( temperature > 0x09) LCD_WriteData(mystr[1]);
				LCD_Cursor(15);
				if( temperature > 0x63) LCD_WriteData(mystr[2]);
				LCD_Cursor(33);
			}
			alarm_state = Alarm_menu;
			break;
		case Alarm_menu:
			if (selection == 'A'){
				if(Alarm_Status == 0x00){
					alarm_state = Alarm_on; 
					break;
				}else{
					alarm_state = Alarm_turn_off;
					delay_ms(100);
					break;
				}
			}
			if (selection == 'B'){
				alarm_state = Alarm_settings_display;
			}else{
				alarm_state = Alarm_menu;
			}
			break;
		case Alarm_on:
			alarm_state = Alarm_menu_display;
			break;
		case Alarm_turn_off:
			alarm_state = Acode1;
			break;
		case Acode1:
			if(key == 0xFF){
				alarm_state = Acode1;
			}else if(key == passcode[0]){
				alarm_state = Acode2;
				LCD_WriteData(key);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);
				alarm_state = Alarm_menu_display;
			}
			break;
		case Acode2:
			if(key == 0xFF){
				alarm_state = Acode2;
			}else if(key == passcode[1]){
				alarm_state = Acode3;
				LCD_WriteData(key);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);
				alarm_state = Alarm_menu_display;
			}
			break;
		case Acode3:
			if(key == 0xFF){
				alarm_state = Acode3;
			}else if(key == passcode[2]){
				alarm_state = code4;
				LCD_WriteData(key);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);
				alarm_state = Alarm_menu_display;
			}
			break;
		case Acode4:
			if(key == 0xFF){
				alarm_state = Acode4;
			}else if(key == passcode[3]){
				alarm_state = Alarm_off;
				LCD_WriteData(key);
				delay_ms(100);
				LCD_DisplayString(1,"Correct!");
				delay_ms(1000);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);
				alarm_state = Alarm_menu_display;
			}
			break;
		case Alarm_off:
			alarm_state = Alarm_menu_display;
			break;
		case Alarm_settings_display:
			alarm_state = Alarm_settings;
			break;
		case Alarm_settings:
			if( selection == 'A'){
				alarm_state = passcode_settings;
				break;
			}else if(selection == 'B'){
				alarm_state = temp_settings;
				break;
			}else if(selection == 'C'){
				alarm_state = Alarm_menu_display;
				break;
			}else{
				alarm_state = Alarm_settings;
			}
			break;
		case temp_settings:
			alarm_state = Alarm_settings_display;
			break;
		case passcode_settings:
			alarm_state = code1;
			break;
		case code1:
			if(key == 0xFF){
				alarm_state = code1;
			}else if(key == passcode[0]){
				alarm_state = code2;
				LCD_WriteData(key);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);	
				alarm_state = Alarm_settings_display;
			}
			break;
		case code2:
			if(key == 0xFF){
				alarm_state = code2;
			}else if(key == passcode[1]){
				alarm_state = code3;
				LCD_WriteData(key);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);	
				alarm_state = Alarm_settings_display;
			}
			break;
		case code3:
			if(key == 0xFF){
				alarm_state = code3;
			}else if(key == passcode[2]){
				alarm_state = code4;
				LCD_WriteData(key);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);	
				alarm_state = Alarm_settings_display;
			}
			break;
		case code4:
			if(key == 0xFF){
				alarm_state = code4;
			}else if(key == passcode[3]){
				alarm_state = Rcode1;
				LCD_WriteData(key);
				LCD_DisplayString(1,"Enter new       passcode:");
				LCD_Cursor(26);
				delay_ms(1000);
			}else{
				LCD_DisplayString(1,"Wrong try again!");
				delay_ms(1000);	
				alarm_state = Alarm_settings_display;
			}
			break;
		case Rcode1:
			if(key == 0xFF){
				alarm_state = Rcode1;
			}else{
				passcode[0] = key;
				alarm_state = Rcode2;
				LCD_WriteData(key);
			}
			break;
		case Rcode2:
			if(key == 0xFF){
				alarm_state = Rcode2;
			}else{
				passcode[1] = key;
				alarm_state = Rcode3;
				LCD_WriteData(key);
			}
			break;
		case Rcode3:
			if(key == 0xFF){
				alarm_state = Rcode3;
			}else{
				passcode[2] = key;
				alarm_state = Rcode3;
				LCD_WriteData(key);
			}
			break;
		case Rcode4:
			if(key == 0xFF){
				alarm_state = Rcode4;
			}else{
				passcode[3] = key;
				alarm_state = Alarm_settings_display;
				LCD_WriteData(key);
				delay_ms(100);
				LCD_DisplayString(1, "New passcode set.");
				delay_ms(1500);
			}
			break;
		default:
			alarm_state = Alarm_menu_display;
			break;
	}
	switch (alarm_state){
		case Alarm_menu_display:
			break;
		case Alarm_menu:
			break;
		case Alarm_on:	
			Alarm_Status = 0x01;
			LCD_DisplayString(1,"Alarm is on!");
			delay_ms(1000);
			break;
		case Alarm_turn_off:
			LCD_DisplayString(1,"Enter           passcode:");
			LCD_Cursor(26);
			break;
		case Alarm_off:
			Alarm_Status = 0x00;
			LCD_DisplayString(1, "Alarm is off!");
			delay_ms(1000);
			break;
		case Alarm_settings_display:
			LCD_DisplayString(1, "A:Reset PasscodeB:Cel/Far C:Back");
			break;
		case Alarm_settings:
			break;
		case passcode_settings:
			LCD_DisplayString(1, "Enter the four   digit code: ");
			LCD_Cursor(29);
			break;
		case temp_settings:
			if (Temp_choice == 'F'){
				LCD_DisplayString(1,"Temperature now in Celsius.");
				Temp_choice = 'C';
				delay_ms(1500);
			}else if(Temp_choice == 'C'){
				LCD_DisplayString(1,"Temperature now in Fahrenheit.");
				Temp_choice = 'F';
				delay_ms(1500);
			}
			break;
		case code1:
			break;
		case code2:
			break;
		case code3:
			break;
		case code4:
			break;
		case Acode1:
			break;
		case Acode2:
			break;
		case Acode3:
			break;
		case Acode4:
			break;
		case Rcode1:
			break;
		case Rcode2:
			break;
		case Rcode3:
			break;
		case Rcode4:
			break;
		default:
			break;
	}
}


// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	// Buttons PORTB[0-7], set AVR PORTA to pull down logic
	DDRB = 0xF0; PORTB = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	//Declare an array of tasks 
	static task  task1, task2;
	task *tasks[] = {  &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	
	// Task 1
	task1.state = 0;//Task initial state.
	task1.period = 200;//Task Period.
	task1.elapsedTime = 1;//Task current elapsed time.
	task1.TickFct = &Alarm_tick;//Function pointer for the tick.
	
	// Task 2
	task2.state = 0;//Task initial state.
	task2.period = 50;//Task Period.
	task2.elapsedTime = 1;//Task current elapsed time.
	task2.TickFct = &keypad;//Function pointer for the tick.
	
	// Set the timer and turn it on
	TimerSet(1);
	TimerOn();
	LCD_init();
	//PWM_on();
	//initialize eventually in memory
	Alarm_Status = 0x00;
	Temp_choice = 'F';
	
	passcode[0] = '1';
	passcode[1] = '2';
	passcode[2] = '3';
	passcode[3] = '4';

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
