
//////////////////////////////////////////////////////////////////////////////
//*************************Check PIR for motion*****************************
/////////////////////////////////////////////////////////////////////////////
enum PIRstates{PIRwait, PIRdetect1,PIRdetect1wait, PIRdetect2, PIRdetect2wait }PIRstate;

int PIR_Tick(){
	unsigned char A1 = ~PINA & 0x02; //PIR sensor
	switch(PIRstate){
		case PIRwait:
			Detected = 0x00;
			if(A1){
				PIRstate = PIRdetect1;
			}else{
				PIRstate = PIRwait;
			}
			break;
		case PIRdetect1:
			if(A1){
				PIRstate = PIRdetect1;
			}else{
				PIRstate = PIRdetect1wait;
			}
			break;
		
		case PIRdetect1wait:
			if(A1){
				PIRstate = PIRdetect2;
				Detected = 0xFF;
			}else{
				PIRstate = PIRdetect1wait;
			}	
			break;
		case PIRdetect2:
			if(A1){
				PIRstate = PIRdetect2;
			}else{
				PIRstate = PIRdetect2wait;
			}
			break;
		case PIRdetect2wait:
			PIRstate = PIRdetect2wait;
			
			break;
		default:
			PIRstate = PIRwait;
			break;
		
	}	
	
}
