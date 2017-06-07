
enum BTstates{ waitSignal }btstate;

int BT_tick(){
	switch(btstate){
		case waitSignal:
		btstate = waitSignal;
		BT = USART_Receive();
		if ( BT == 'A'){
			Beep_on = 0xFF;
			}else if( BT == 'B'){
			Beep_on = 0x00;
			}else{
			Beep_on = Beep_on;
		}
		break;
	}
}
