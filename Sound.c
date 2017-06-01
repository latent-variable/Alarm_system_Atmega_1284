
///////////////////////////////////////////////////////////////////////////////////////
//******************************Generate the Alarm triggered sound********************
//////////////////////////////////////////////////////////////////////////////////////

enum SoundStates{waitdetect, sound1, sound2, reset }SoundState;
int Sound_Tick(){
	unsigned char sound;
	switch(SoundState){
		case waitdetect:
			if(Alarm_Status == 0x01 && Detected == 0xFF ){
				SoundState = sound1;
			}else{
				SoundState = waitdetect;
			}
			break;
		case sound1:
			if(Alarm_Status == 0x00 ){
				SoundState = reset;
			}else{
				SoundState = sound2;
			}
			break;
		case sound2:
			if(Alarm_Status == 0x00 ){
				SoundState = reset;
			}else{
				SoundState = sound1;
			}
			break;
		case reset:
			SoundState = waitdetect;
			break;
		default:
			SoundState = waitdetect;
			break;
			
	}
	switch(SoundState){
		case waitdetect:
			break;
		case sound1:
			sound = 0x00;
			PORTD = sound &0x10;
			break;
		case sound2:
			sound = 0xFF;
			PORTD = sound &0x10;
			break;
		case reset:
			break;
		default:
			break;
	}
}

enum BeepStates{waitBeep, Beep1, Beep2}beepstate;
int Beep_tick(){
	static unsigned char beepcnt;
	unsigned char sound;
	switch(beepstate){
		case waitBeep:
			if(Beep_on == 0xFF){
				beepstate = Beep1;
			}else{
				beepstate = waitBeep;
			}
			break;
		case Beep1:
			if(Beep_on == 0xFF){
				beepstate = Beep2;
			}else{
				beepstate = waitBeep;
			}
			break;
		case Beep2:
			if(Beep_on == 0xFF){
				beepstate = Beep1;
			}else{
				beepstate = waitBeep;
			}
			break;
		default:
			beepstate = waitBeep;
	}
	switch(beepstate){
		case waitBeep:
			break;
		case Beep1:
			sound = 0xFF;
			PORTD = sound &0x10;
			break;
		case Beep2:
			sound = 0x00;
			PORTD = sound &0x10;
			break;
		default:
			break;
			
	}
	
};
