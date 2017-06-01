
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*********Main alarm system Tick function***********************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum AlarmStates{Alarm_menu_display, Alarm_menu,Alarm_on,Alarm_triggered1,Alarm_triggered2, Alarm_turn_off, Alarm_off,Alarm_settings_display, Alarm_settings, temp_settings, passcode_settings,code1,code2,code3,code4, Acode1,Acode2,Acode3,Acode4,Rcode1,Rcode2,Rcode3,Rcode4, }alarm_state;
	
int Alarm_tick(){
	unsigned char selection = key;
	unsigned char temperature = 72;
	char mystr[5];
	static unsigned char wrong;
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
			Beep_on = 0x00;
			if(Alarm_Status != 0x00){
				if(Detected == 0xFF){
					alarm_state = Alarm_triggered1;
					break;
				}
			}
			if (selection == 'A'){
				if(Alarm_Status == 0x00){
					alarm_state = Alarm_on; 
					Beep_on = 0xFF;
					break;
				}else{
					alarm_state = Alarm_turn_off;
					Beep_on = 0xFF;
					break;
				}
				
			}
			if (selection == 'B'){
				alarm_state = Alarm_settings_display;
				Beep_on = 0xFF;
			}else{
				alarm_state = Alarm_menu;
			}
			break;
		case Alarm_on:
			Beep_on = 0x00;
			alarm_state = Alarm_menu_display;
			break;
		case Alarm_triggered1:
			alarm_state = Alarm_triggered2;
			break;
		case Alarm_triggered2:
			alarm_state = Acode1;
			break;
		case Alarm_turn_off:
			Beep_on = 0x00;
			alarm_state = Acode1;
			break;
		case Acode1:
			wrong = 0x00;
			if(key == 0xFF){
				alarm_state = Acode1;
			}else if(key == passcode[0]){
				alarm_state = Acode2;
				LCD_WriteData('*');
			}else{
				wrong = 0xFF;
				LCD_WriteData('*');
				alarm_state = Acode2;
			}
			break;
		case Acode2:
			if(key == 0xFF){
				alarm_state = Acode2;
			}else if(key == passcode[1]){
				alarm_state = Acode3;
				LCD_WriteData('*');
			}else{
				wrong = 0xFF;
				LCD_WriteData('*');
				alarm_state = Acode3;
			}
			break;
		case Acode3:
			if(key == 0xFF){
				alarm_state = Acode3;
			}else if(key == passcode[2]){
				alarm_state = Acode4;
				LCD_WriteData('*');
			}else{
				wrong = 0xFF;
				LCD_WriteData('*');
				alarm_state = Acode4;
			}
			break;
		case Acode4:
			if(key == 0xFF){
				alarm_state = Acode4;
			}else if(key == passcode[3] && wrong == 0x00){
				alarm_state = Alarm_off;
				LCD_WriteData('*');
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
			Beep_on = 0x00;
			if(Alarm_Status != 0x00){
				if(Detected == 0xFF){
					alarm_state = Alarm_triggered1;
					break;
				}
			}
			if( selection == 'A'){
				alarm_state = passcode_settings;
				Beep_on = 0xFF;
				break;
			}else if(selection == 'B'){
				alarm_state = temp_settings;
				Beep_on = 0xFF;
				break;
			}else if(selection == 'C'){
				alarm_state = Alarm_menu_display;
				Beep_on = 0xFF;
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
			Beep_on = 0x00;
			wrong = 0x00;
			if(key == 0xFF){
				alarm_state = code1;
			}else if(key == passcode[0]){
				alarm_state = code2;
				LCD_WriteData('*');
			}else{
				wrong = 0xFF;
				LCD_WriteData('*');
				alarm_state = code2;
			}
			break;
		case code2:
			if(key == 0xFF){
				alarm_state = code2;
			}else if(key == passcode[1]){
				alarm_state = code3;
				LCD_WriteData('*');
			}else{
				wrong = 0xFF;
				LCD_WriteData('*');
				alarm_state = code3;
			}
			break;
		case code3:
			if(key == 0xFF){
				alarm_state = code3;
			}else if(key == passcode[2]){
				alarm_state = code4;
				LCD_WriteData('*');
			}else{
				wrong = 0xFF;
				LCD_WriteData('*');
				alarm_state = code4;
			}
			break;
		case code4:
			if(key == 0xFF){
				alarm_state = code4;
			}else if(key == passcode[3] && wrong == 0x00){
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
				alarm_state = Rcode4;
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
				LCD_DisplayString(1, "New passcode set");
				delay_ms(2000);
			}
			break;
		default:
			alarm_state = Alarm_menu_display;
			break;
	}
	
	///////////////////////////////////////////////////////////////////
	//**************************Actions*********************************
	/////////////////////////////////////////////////////////////////
	switch (alarm_state){
		case Alarm_menu_display:
			break;
		case Alarm_menu:
			break;
		case Alarm_triggered1:
			LCD_DisplayString(1,"Alarm triggered!Enter code:");
			LCD_Cursor(28);
		case Alarm_triggered2:
			break;
		case Alarm_on:	
			PIRstate = PIRwait;
			Alarm_Status = 0x01;
			LCD_DisplayString(1,"Alarm is on!");
			delay_ms(1000);
			break;
		case Alarm_turn_off:
			LCD_DisplayString(1,"Enter           passcode:");
			LCD_Cursor(26);
			break;
		case Alarm_off:
			Detected = 0x00;
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
			LCD_DisplayString(1, "Enter           passcode:");
			LCD_Cursor(26);
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
