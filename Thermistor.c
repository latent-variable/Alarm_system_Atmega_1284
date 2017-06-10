///////////////////////////////////////////////////////////////////////////////////
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

///////////////////////////////////////////////////////////////////////////////
//The following code was used and modifyed with permision of  Chris Lockfort
/* Thermistor reader program for AVR chips
*  By Chris Lockfort (clockfort@csh.rit.removethispartforspam.edu)
* 
*  Basic Schematic:
*
*  GND --- 10K ohm normal resistor ---|--- 10K ohm thermistor ---- VCC
*  				      |
*  				Microcontroller ADC
*
* I haven't back-of-the-napkin'd the math, but I believe you might want to 
* use a lower-tolerance resistor than you would normally care about.
*
* Portions of this code are borrowed from Paul J Stoffregen's sample ADC code,
* many thanks to him for providing readable examples.
*/

//Copyright (c) 2010  Chris Lockfort
int16_t read_analog_input(){
	ADCSRA = (1<<ADEN) | (1<<ADSC);
	while (ADCSRA & (1<<ADSC)) ; //block until ready
	uint8_t lower_byte = ADCL;
	int16_t ret_val = ADCH << 8;
	ret_val |= lower_byte;
	return ret_val;
}

int Temp_tick(){
	//tempstate = sample;
	unsigned char A0 = PINA & 0x01;
	/*switch(tempstate){
		
	//case sample:
	ADC = (A0 * 1024) / 5 ;
	unsigned char x = ADC;
	
	if(Temp_choice == 'F'){
		Temp = x;
	}else{
		Temp = (x - 32.0)* (5.0/9.0);
	}*/
	//Copyright (c) 2010  Chris Lockfort
	
	float reading = read_analog_input();
	// I apologize for the following code, it's based off of an Arduino Playground example
	// It seems to mumble something about being a 10K-ohm specific version of a Steinhart-Hart Thermistor Equation :-)
	reading = log(((10240000/reading) - 10000));
	reading = 1 / (0.001129148 + (0.000234125 * reading) + (0.0000000876741 * reading * reading * reading));
	reading -= 282.55; //K to C
	
	
	if(Temp_choice == 'F'){
		Temp = (reading * 9.0)/ 5.0 + 32.0;;
	}else{
		Temp = reading;
	}

}
