#include"UART.h"
#include"vehicle.h"

//#define _TRACING_

int main()
{
	Front_bool Mods;	
	UART_init();
	rbusy=false;	
	Car_Initi();
   //if(UART_bit9)
		SendString(" \"���ӳɹ�\" \r\n");
	while(1){
		if(rbusy)//ʹ�����봮��״̬�ж�
		{
			if(Rear_MiddleTracking)
				SendString(" ���洫���������� \r\n");
					
			Mods=SpeedMode_Din(info);
			if(Mods == true)
			   Option_prompt();
			else if(Mods == blck)
				Black_Tracking_Action();
		}
				
		
		while(!UART_bit9){
			Vehicle_Stop();
			delay_ms(10);
			if(rbusy)
				SendString("Not connected \r\n");	
			rbusy=false;	
		}
		
	 }			 

	return 0;

}

