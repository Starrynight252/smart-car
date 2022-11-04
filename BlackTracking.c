/*��ɫ����->Ѱ�� Black tracking */
/*########���ļ�ʹ��"vehicle.h"ͷ�ļ�##*/
#include"vehicle.h"
#include"UART.h"
/*#################���ñ���#######################*/
extern uchar Mode_Selection;  //ģʽѡ��
extern uint MOTOR_PwmTime,SE_PwmTime;   //PWMʱ��
extern uint MOTOR_Pwmcount,SE_PwmCount;  //PWM���Ƶ��


void Black_Tracking_Action()
{
	uint RestrictedMode='.',infos;

	Car_Initi();//��ʼ��
	SendString("�켣���� \n\r");
	MOTOR_Pwmcount=66;

   	while(1)
	{
		//���ڲ��� ִ��ǰ���� ���� 0
		if(rbusy)//ʹ�����봮��״̬�ж�
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

		/*Ѱ�������߼�*/ 

		//ͣ����� ִ��ǰ���� ����1
		if(!Front_MiddleTracking &&(!FrontL_MiddleTracking && !FrontR_MiddleTracking)) //!Front_MiddleTracking   
		{
			SendString("NO Trajectories\r\n");
			Vehicle_Stop();	
		}

		//������� ִ��ǰ��ͣ�� ����2 
		if((Mode_Selection=='P' || Mode_Selection=='F') && Front_MiddleTracking)
		{
			Vehicle_ForwardRotation();//ֱ��
			Option_prompt();	 
		}

		//ת���⣬ִ��ǰ������ ����3
		if(Mode_Selection=='W')
		{	
			//ת��ִ��ǰ��δת��
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

			//����ִ��ǰ��ת����
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


