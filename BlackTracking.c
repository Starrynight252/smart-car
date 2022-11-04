/*黑色跟踪->寻迹 Black tracking */
/*########该文件使用"vehicle.h"头文件##*/
#include"vehicle.h"
#include"UART.h"
/*#################引用变量#######################*/
extern uchar Mode_Selection;  //模式选择
extern uint MOTOR_PwmTime,SE_PwmTime;   //PWM时间
extern uint MOTOR_Pwmcount,SE_PwmCount;  //PWM电机频率


void Black_Tracking_Action()
{
	uint RestrictedMode='.',infos;

	Car_Initi();//初始化
	SendString("轨迹程序 \n\r");
	MOTOR_Pwmcount=66;

   	while(1)
	{
		//串口操作 执行前提无 优先 0
		if(rbusy)//使用输入串口状态判断
		{
			infos=info;
			if(infos != 'B'){
				if(SpeedMode_Din(infos))
			   		Option_prompt();
			}
			else 
			{
				rbusy=false;
				Vehicle_Stop();
				SendString("Sign Out\r\n");	
				return ;
			}
		}

		/*寻迹基础逻辑*/ 

		//停车检测 执行前提无 优先1
		if(!Front_MiddleTracking &&(!FrontL_MiddleTracking && !FrontR_MiddleTracking)) //!Front_MiddleTracking   
		{
			SendString("NO Trajectories\r\n");
			Vehicle_Stop();	
		}

		//启动检测 执行前提停车 优先2 
		if((Mode_Selection=='P' || Mode_Selection=='F') && Front_MiddleTracking)
		{
			Vehicle_ForwardRotation();//直行
			Option_prompt();	 
		}

		//转向检测，执行前提启动 优先3
		if(Mode_Selection=='W')
		{	
			//转向，执行前提未转向
			if(RestrictedMode =='.')
			{					
				if(FrontL_MiddleTracking && !FrontR_MiddleTracking)
				{	
					Parameter_Steering('L');
					RestrictedMode='L';
					delay_nus(8);
				   SendString("|L|");
				}
				else if(FrontR_MiddleTracking && !FrontL_MiddleTracking)
				{
					Parameter_Steering('R');
					RestrictedMode='R';
					delay_nus(8);
					SendString("|R|");
				}
			}

			//回向，执行前提转过向
			if(Front_MiddleTracking&& (!FrontL_MiddleTracking && !FrontR_MiddleTracking))
			{
				if(RestrictedMode =='L')
				{
					Parameter_Steering('R');
					SendString("|-R|\r\n");
					RestrictedMode='.';
				}
				else if(RestrictedMode =='R')
				{ 
					Parameter_Steering('L');
					SendString("|-L|\r\n");
					RestrictedMode='.';	
				}
			}	

		}			
					
	}


}


