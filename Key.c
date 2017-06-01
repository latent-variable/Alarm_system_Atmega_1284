
////////////////////////////////////////////////////////////////////
//**********************Get desired input*************************
/////////////////////////////////////////////////////////////////
enum Keypad_States{wait_input, input_press, input }key_state;
int keypad(){
	unsigned char x;
	static unsigned char y;
	x = GetKeypadKey();
	switch(key_state){
			case wait_input:
				if( x  == '\0' ){
					key_state = wait_input;
					key = 0xFF;
				}else{
					key_state = input_press;
					y = x;
				}
				break;
			case input_press:
				if(x == '\0'){
					key_state = input;
					key = y;
				}else{
					key_state = input_press;
				}
				break;
			case input:
				key_state = wait_input;
			
				break;
	}
}
