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
		SendString(" \"连接成功\" \r\n");
	while(1){
		if(rbusy)//使用输入串口状态判断
		{
			if(Rear_MiddleTracking)
				SendString(" 火焰传感器被激活 \r\n");
					
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

