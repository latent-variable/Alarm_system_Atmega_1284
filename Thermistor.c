///////////////////////////////////////////////////////////////////////////////////
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}
//enum TempStates{sample}tempstate;

int Temp_tick(){
	//tempstate = sample;
	unsigned char A0 = PINA & 0x01;
	//switch(tempstate){
	//case sample:
	ADC = (A0 * 1024) / 5 ;
	unsigned char x = ADC;
	
	if(Temp_choice == 'F'){
		Temp = x;
		}else{
		Temp = (x - 32)* (5/9) ;
	}
	//break;
	//default:
	//tempstate = sample;
	//break;
	
	//}
}
