#include"vehicle.h"
#include"UART.h"

/*#################定义变量#######################*/
uchar Mode_Selection;  //模式选择
/*PWM占空比
*当xx_pwmcount为20时
*因为if(Pwm_EMtime<=Pwm_EMcount) pwmx=1 else pwm=0;
*所以%20的时间输出高电平,剩余%输出低电平
*/
uint MOTOR_PwmTime,SE_PwmTime;   //PWM时间
uint MOTOR_Pwmcount,SE_PwmCount;  //PWM电机频率
	 
/*----------------------车辆初始化--------------------------*/
void Car_Initi()
{
	P2=LAMP_P2_INIT;  //P2初始化
	P1=P1_INIT;  //P1初始化
	
	/* 变量初始化 */
	//状态
	Brake_Beam=LAMP_ON; //刹车灯开		
	EM_OUT3=EM_OUT4=EM_OUT1=EM_OUT2=1;
	//变量
	Mode_Selection='F';  //模式选择
	MOTOR_Pwmcount =80;
	SE_PwmCount=SE_TIMINGLNITIAL;  //PWM电机频率
	MOTOR_PwmTime=0;
	SE_PwmTime=0;   //PWM时间

	//定时器
	EA=1;
	TR0=ET0=0;
	
	TH0=0xff;//(65536-10)/256;//赋初值定时
	TL0=0xf7;//(65536-10)%256;//0.01ms
	TMOD|=0x01;
	TR0=1;
	ET0=1;
}

/*停止*/
void Vehicle_Stop()
{
	EA=0;
	Inverse_Beam= LAMP_OFF;
	Brake_Beam=LAMP_ON; //刹车灯开		
	EM_OUT3=EM_OUT4=EM_OUT1=EM_OUT2=1;
	
	Mode_Selection='P';
	EA=1;
}
/*正转*/
void Vehicle_ForwardRotation()
{
	EM_OUT2=EM_OUT4=0;
	Inverse_Beam= Brake_Beam =LAMP_OFF;  //刹车,倒车灯关	
	Mode_Selection='W';
}
/*反转*/
void Vehicle_Reverse()
{
	EM_OUT1=EM_OUT3=0;
	Brake_Beam =LAMP_OFF;  //刹车灯关
	Inverse_Beam=LAMP_ON; //倒车灯开	
	Mode_Selection='S';
}				  
//转向灯
void Cornering_Lamp()
{	 	
	 //对比SE_TIMINGLNITIAL初始位置
  	if(SE_PwmCount == SE_TIMINGLNITIAL)
	{	//舵机在初始位置,关闭左右灯
		Right_Beam=Left_Beam=LAMP_OFF;
	}
	else if(SE_PwmCount < SE_TIMINGLNITIAL) 
	{	/*右转*/
		Right_Beam=LAMP_ON;
		Left_Beam=LAMP_OFF;
	}
	else if(SE_PwmCount > SE_TIMINGLNITIAL)	
	{	 /*左转*/
		Left_Beam=LAMP_ON;
		Right_Beam=LAMP_OFF;
	}
				
}
//转向控制-根据参数
void Parameter_Steering(const uchar direction)
{
	if(direction=='O')
	{
		SE_PwmCount=SE_TIMINGLNITIAL;  //舵机初始
	}
	else if(direction== 'L')
	{	 /*左转*/
		++SE_PwmCount;
	}
	else if(direction == 'R')
	{	 /*右转*/
		--SE_PwmCount;
	}
	Cornering_Lamp();

}
//转向保持时间
Front_bool Steering_HoldingTime(const uchar direction,const uint keepus,const uint angle)
{
	uint frequency;
	if(direction=='L'||direction=='R')
	{	//计算区间
		frequency=SE_RANGEMAX-SE_TIMINGLNITIAL;
		if(angle && angle <= frequency)
		{  //限制范围
			for(frequency=0;frequency < angle;frequency++)
			{
				Parameter_Steering(direction);			
			}
			delay_nus(keepus);//保持时间
			for(frequency=0;frequency < angle;frequency++)
			{
				/*回归*/
				if(direction=='L')
					Parameter_Steering('R');	
				else
				Parameter_Steering('L');
			}
			return true;	
		}
	}						 
	 
	//超过限制范围||direction! 'L'/'R'
	return false;		
}

//停车设置速度参数
uint Stop_SettingSpeed_Parameters(uint Modernspeed)
{
	uint Pastspeed=MOTOR_Pwmcount;
	Vehicle_Stop();	//停车

	MOTOR_Pwmcount=Modernspeed;
	return Pastspeed;
}

/**设置速度-根据输入的数值
*/
Front_bool Speed_Value()
{
	uint speednum[3],ergodic,speed=0,length=0;

	SendString("请输入速度值(0-200):\r\n");
	length=Get_InputNumber_UintType(speednum,3);
	if(length)
	{
		for(ergodic=0;ergodic<length;ergodic++)
			speed=(speed * 10)+speednum[ergodic];
	}
	
	MOTOR_Pwmcount=speed;
	return true;
}

//速度模式/方向选择
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
		 	} //注:Pwm_EMcount是unsigned
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
		else	//防止以上满足后执行switch
		switch(command)
		{
			case'W':
				if(Mode_Selection=='S')//保护
					Vehicle_Stop();		
				Vehicle_ForwardRotation();
				break;
			case'S':
				if(Mode_Selection=='W')//保护
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
			case'H': High_Beam=~High_Beam;  //远光灯
				break;
			case'L': Low_Beam =~Low_Beam; //近光灯
				break; 
			case'Q': R_Horn =~R_Horn;//喇叭-提醒
				break;	 
			case'c': Peripheral =~Peripheral; 	//外围设备 
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
			SendString("无该指令\r\n");
			rbusy=false;
			return false;	
		}

		rbusy=false;
		return true;
	}
	return false;
}
//命令选项提示
void Option_prompt()
{
	SendString("->Speed|angle|\r\n->  ");
	SendByte(Mode_Selection);

	 //对比SE_TIMINGLNITIAL初始位置
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
/*************************字符串命令*********************************/
Front_bool CommandProcessing()
{
	uchar comarr[ARRAY_SIZE];
	uint range=0,ranges=0;

	SendString("String: "); 

	while(info!=')'||range <= ARRAY_SIZE-1)
	{//结束字符
		if(rbusy)
		{
			comarr[range]=info;
			SendByte(comarr[range++]);
			rbusy=false;
		}	
		delay_nus(2); //延时,
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
		delay_nus(50); //延时,
	}
	
		//delay_nus(2);
	return true;
} 


/*****************************中断********************************/
void PWM_ElectricMachinery() interrupt 1  
{
		
	 TR0=0;		//赋初值时，关闭定时器
     TH0=0xff;	//(65536-10)/256;//赋初值定时
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
	
	if(MOTOR_PwmTime<= MOTOR_Pwmcount )	 //占空比由Pwm_EMcount值控制高电平
	{	
		if(Mode_Selection=='W')
			EM_OUT1=EM_OUT3=1;
		if(Mode_Selection=='S')
			EM_OUT2=EM_OUT4=1;
	}
	else
	{  
		if(Mode_Selection=='W')	 //占空比由if控制低电平
			EM_OUT1=EM_OUT3=0;
		if(Mode_Selection=='S')
			EM_OUT2=EM_OUT4=0;
	}
	if(MOTOR_PwmTime>=MOTOR_TIMINGMAX)   //马达计时上限
		MOTOR_PwmTime=0;		
} 