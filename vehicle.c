#include"vehicle.h"
#include"UART.h"

/*#################�������#######################*/
uchar Mode_Selection;  //ģʽѡ��
/*PWMռ�ձ�
*��xx_pwmcountΪ20ʱ
*��Ϊif(Pwm_EMtime<=Pwm_EMcount) pwmx=1 else pwm=0;
*����%20��ʱ������ߵ�ƽ,ʣ��%����͵�ƽ
*/
uint MOTOR_PwmTime,SE_PwmTime;   //PWMʱ��
uint MOTOR_Pwmcount,SE_PwmCount;  //PWM���Ƶ��
	 
/*----------------------������ʼ��--------------------------*/
void Car_Initi()
{
	P2=LAMP_P2_INIT;  //P2��ʼ��
	P1=P1_INIT;  //P1��ʼ��
	
	/* ������ʼ�� */
	//״̬
	Brake_Beam=LAMP_ON; //ɲ���ƿ�		
	EM_OUT3=EM_OUT4=EM_OUT1=EM_OUT2=1;
	//����
	Mode_Selection='F';  //ģʽѡ��
	MOTOR_Pwmcount =80;
	SE_PwmCount=SE_TIMINGLNITIAL;  //PWM���Ƶ��
	MOTOR_PwmTime=0;
	SE_PwmTime=0;   //PWMʱ��

	//��ʱ��
	EA=1;
	TR0=ET0=0;
	
	TH0=0xff;//(65536-10)/256;//����ֵ��ʱ
	TL0=0xf7;//(65536-10)%256;//0.01ms
	TMOD|=0x01;
	TR0=1;
	ET0=1;
}

/*ֹͣ*/
void Vehicle_Stop()
{
	EA=0;
	Inverse_Beam= LAMP_OFF;
	Brake_Beam=LAMP_ON; //ɲ���ƿ�		
	EM_OUT3=EM_OUT4=EM_OUT1=EM_OUT2=1;
	
	Mode_Selection='P';
	EA=1;
}
/*��ת*/
void Vehicle_ForwardRotation()
{
	EM_OUT2=EM_OUT4=0;
	Inverse_Beam= Brake_Beam =LAMP_OFF;  //ɲ��,�����ƹ�	
	Mode_Selection='W';
}
/*��ת*/
void Vehicle_Reverse()
{
	EM_OUT1=EM_OUT3=0;
	Brake_Beam =LAMP_OFF;  //ɲ���ƹ�
	Inverse_Beam=LAMP_ON; //�����ƿ�	
	Mode_Selection='S';
}				  
//ת���
void Cornering_Lamp()
{	 	
	 //�Ա�SE_TIMINGLNITIAL��ʼλ��
  	if(SE_PwmCount == SE_TIMINGLNITIAL)
	{	//����ڳ�ʼλ��,�ر����ҵ�
		Right_Beam=Left_Beam=LAMP_OFF;
	}
	else if(SE_PwmCount < SE_TIMINGLNITIAL) 
	{	/*��ת*/
		Right_Beam=LAMP_ON;
		Left_Beam=LAMP_OFF;
	}
	else if(SE_PwmCount > SE_TIMINGLNITIAL)	
	{	 /*��ת*/
		Left_Beam=LAMP_ON;
		Right_Beam=LAMP_OFF;
	}
				
}
//ת�����-���ݲ���
void Parameter_Steering(const uchar direction)
{
	if(direction=='O')
	{
		SE_PwmCount=SE_TIMINGLNITIAL;  //�����ʼ
	}
	else if(direction== 'L')
	{	 /*��ת*/
		++SE_PwmCount;
	}
	else if(direction == 'R')
	{	 /*��ת*/
		--SE_PwmCount;
	}
	Cornering_Lamp();

}
//ת�򱣳�ʱ��
Front_bool Steering_HoldingTime(const uchar direction,const uint keepus,const uint angle)
{
	uint frequency;
	if(direction=='L'||direction=='R')
	{	//��������
		frequency=SE_RANGEMAX-SE_TIMINGLNITIAL;
		if(angle && angle <= frequency)
		{  //���Ʒ�Χ
			for(frequency=0;frequency < angle;frequency++)
			{
				Parameter_Steering(direction);			
			}
			delay_nus(keepus);//����ʱ��
			for(frequency=0;frequency < angle;frequency++)
			{
				/*�ع�*/
				if(direction=='L')
					Parameter_Steering('R');	
				else
				Parameter_Steering('L');
			}
			return true;	
		}
	}						 
	 
	//�������Ʒ�Χ||direction! 'L'/'R'
	return false;		
}

//ͣ�������ٶȲ���
uint Stop_SettingSpeed_Parameters(uint Modernspeed)
{
	uint Pastspeed=MOTOR_Pwmcount;
	Vehicle_Stop();	//ͣ��

	MOTOR_Pwmcount=Modernspeed;
	return Pastspeed;
}

/**�����ٶ�-�����������ֵ
*/
Front_bool Speed_Value()
{
	uint speednum[3],ergodic,speed=0,length=0;

	SendString("�������ٶ�ֵ(0-200):\r\n");
	length=Get_InputNumber_UintType(speednum,3);
	if(length)
	{
		for(ergodic=0;ergodic<length;ergodic++)
			speed=(speed * 10)+speednum[ergodic];
	}
	
	MOTOR_Pwmcount=speed;
	return true;
}

//�ٶ�ģʽ/����ѡ��
Front_bool SpeedMode_Din(uchar command)
{	
	if(rbusy)
	{				   
		if((command=='X'||command=='Z')||(command=='+'||command=='-'))
		{	  
			if(command=='+'){
		 		if(( MOTOR_Pwmcount +1) <= MOTOR_TIMINGMAX)
					++ MOTOR_Pwmcount ;
		 	}	
			 else if(command=='Z'){	 
		 		if(( MOTOR_Pwmcount +10) <= MOTOR_TIMINGMAX)
		 		 	 MOTOR_Pwmcount +=10;
		 	} //ע:Pwm_EMcount��unsigned
		 	else if(command=='-'){
		 		if(( MOTOR_Pwmcount -1) <= MOTOR_TIMINGMAX)
					-- MOTOR_Pwmcount ;			
		 	} 
		 	else if(command=='X'){
		 		if(( MOTOR_Pwmcount -10) <= MOTOR_TIMINGMAX)
		 	 		 MOTOR_Pwmcount -=10;
			}	
		}
		else if(command=='(')
		{
			return CommandProcessing();
		}
		else	//��ֹ���������ִ��switch
		switch(command)
		{
			case'W':
				if(Mode_Selection=='S')//����
					Vehicle_Stop();		
				Vehicle_ForwardRotation();
				break;
			case'S':
				if(Mode_Selection=='W')//����
					Vehicle_Stop();
				Vehicle_Reverse();
				break;
			case'P':Vehicle_Stop();break;
	  	 	case'A':Parameter_Steering('L');break;
			case'D':Parameter_Steering('R');break;
			case'O':Parameter_Steering('O');break;
			case'F':Car_Initi(); break;
			case'B':
				rbusy=false;
				return blck;//Black_Tracking_Action(); break;
			case'H': High_Beam=~High_Beam;  //Զ���
				break;
			case'L': Low_Beam =~Low_Beam; //�����
				break; 
			case'Q': R_Horn =~R_Horn;//����-����
				break;	 
			case'c': Peripheral =~Peripheral; 	//��Χ�豸 
				break;	 
			
			case'0': Custom_zero=~Custom_zero; 
				break;
			case'1': Custom_One=~Custom_One;
				break;
			case'2': Custom_two=~Custom_two;
				break;
			case'3':  Custom_three=~Custom_three;
				break;
			case'4':  Custom_four=~Custom_four;
				break;
			case'5':  Custom_five=~Custom_five;
				break;
			case'6':  Custom_six=~Custom_six;
				break;
			case'7':  Custom_seven=~Custom_seven;
				break;

			default:
			SendString("�޸�ָ��\r\n");
			rbusy=false;
			return false;	
		}

		rbusy=false;
		return true;
	}
	return false;
}
//����ѡ����ʾ
void Option_prompt()
{
	SendString("->Speed|angle|\r\n->  ");
	SendByte(Mode_Selection);

	 //�Ա�SE_TIMINGLNITIAL��ʼλ��
	if(SE_PwmCount&& SE_PwmCount< SE_TIMINGLNITIAL )
		SendString("+D");
	else if(SE_PwmCount> SE_TIMINGLNITIAL)
		SendString("+A");
		
	SendString("| ");
	SendNum( MOTOR_Pwmcount );
	SendString("| ");
	SendNum(SE_PwmCount);
	SendString("\r\n");
	delay_nus(2);
}		  
/*************************�ַ�������*********************************/
Front_bool CommandProcessing()
{
	uchar comarr[ARRAY_SIZE];
	uint range=0,ranges=0;

	SendString("String: "); 

	while(info!=')'||range <= ARRAY_SIZE-1)
	{//�����ַ�
		if(rbusy)
		{
			comarr[range]=info;
			SendByte(comarr[range++]);
			rbusy=false;
		}	
		delay_nus(2); //��ʱ,
	 }
	rbusy=false;
	SendString("\r\n");

	ranges=range;
	for(range=0;range < ranges;)
	{
		SendNum(range);
		SpeedMode_Din(comarr[range]);
		SendByte(comarr[range++]);	
		SendString("\r\t");
		delay_nus(50); //��ʱ,
	}
	
		//delay_nus(2);
	return true;
} 


/*****************************�ж�********************************/
void PWM_ElectricMachinery() interrupt 1  
{
		
	 TR0=0;		//����ֵʱ���رն�ʱ��
     TH0=0xff;	//(65536-10)/256;//����ֵ��ʱ
     TL0=0xf7;	//(65536-10)%256;//0.01ms
	 TR0=1;

	SE_PwmTime++;
	if(SE_PwmTime <=SE_PwmCount)
		SE_OUT1=0;
	else
		SE_OUT1=1;

	if(SE_PwmTime >= SE_TIMINGMAX)
		SE_PwmTime=0;

	MOTOR_PwmTime++;        
	
	if(MOTOR_PwmTime<= MOTOR_Pwmcount )	 //ռ�ձ���Pwm_EMcountֵ���Ƹߵ�ƽ
	{	
		if(Mode_Selection=='W')
			EM_OUT1=EM_OUT3=1;
		if(Mode_Selection=='S')
			EM_OUT2=EM_OUT4=1;
	}
	else
	{  
		if(Mode_Selection=='W')	 //ռ�ձ���if���Ƶ͵�ƽ
			EM_OUT1=EM_OUT3=0;
		if(Mode_Selection=='S')
			EM_OUT2=EM_OUT4=0;
	}
	if(MOTOR_PwmTime>=MOTOR_TIMINGMAX)   //����ʱ����
		MOTOR_PwmTime=0;		
} 