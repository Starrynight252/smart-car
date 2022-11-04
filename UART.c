#include"UART.h"

/*#################定义变量#######################*/
Front_bool busy,rbusy;  //串口状态
uchar info='*';	   //串口缓存

void delay_nus(uint time) 
{
    unsigned int i=0;
        for (i=0;i<time;i++)
            _nop_();
}
void delay_ms(uint time)   
{
 uint a,b;
	for(a=time;a>0;a--)
		for(b=110;b>0;b--); //延时
}
/**************UART************************/
void UART_init()
{
	rbusy=false;
	TMOD|=0x20; //设置定时器1工作在模式2作为串口通讯
	/*RCAP2L =TL2=(UCHAR_MAX-(FOSC/32/BAUD));		//UART
	//RCAP2H =TH2=(UCHAR_MAX-(FOSC/32/BAUD))>>8;
	//PT1=1;	
	T2CON=0x34;
	//PT0=0;
	*/
	IP=0x10;
	//根据STC官网提供的波特率计算公式
	TH1=TL1=-(FOSC/12/32/BAUD); 
	 
	#if(PARITYBIT==0)
		SCON=0X50; 	
	#elif(PARITYBIT==1)
		SCON=0Xda;
	#elif(PARITYBIT==2)
		SCON=0Xd2; 
	#endif

	TR1=1;//启动T1  
	
	ES=1; //响应中断
	EA=1; //开放中断
}
//结束标志
Front_bool End_Flag()
{	
	if(info==0x0D){//回车
		rbusy=false;
		return true;
	}
	return false;
}
/***************接受字节********************************/
//1.使用rbusy判断,true为已经获得1字节数据,相反false为没有获得1字节数据
/*2.Get_InputString 
*Front_bool Get_InputString_UcharType(uchar *array,const uint length);
*
*/
uint Get_InputString_UcharType(uchar *array,const uint length)
{
	uint ergodic;
	rbusy=false;

	for(ergodic=0; ergodic < length;)
	{
		if(rbusy){
			if(End_Flag())
				return ergodic;
			array[ergodic++]=info;
			rbusy=false;
		}
		
	}
	array[ergodic]='\0';
	return ergodic;
} 
uint Get_InputNumber_UintType(uint *array,const uint length)
{
	/*因为不支持VLA(C99),所以无法简单使用Get_InputString_UcharType()
	因为所需要的数组元素长度无法确定.*/
	//uint arr[length];
	uchar ergodic,overtime=0;
	rbusy=false;	 

	for(ergodic=0;ergodic<length;)
	{
		if(rbusy){
			if(End_Flag())
				return ergodic; 	
			array[ergodic++]=(info%48);
			rbusy=false;				
		}
	} 
		return ergodic;  
}
/***************发送字节********************************/
void SendByte(const uchar date)
{
	while(busy==true);
		ACC=date;  

	if(P)
	{	
		#if(PARITYBIT==1)
			TB8=0;
		#elif(PARITYBIT==2)
			TB8=1; 
		#endif
	}
	else
	{
		#if(PARITYBIT==1)
			TB8=1;
		#elif(PARITYBIT==2)
			TB8=0; 
		#endif
	}
	busy=true;
	SBUF=ACC;
}
//发送字符串
void SendString(const uchar* date)
{
	while(*date)
	 SendByte(*date++);	
//	SendByte('\0');	
}
//发送整(int)数
void SendNum(uint i)
{
	int a[10];
	int j=0;
	if(!i)
	{
		SendByte(0x30);
	}
	else
	{
		while(i > 0)
		{
			a[j++]=i%10+'0';
			i/=10;
		}
	}
	for(j--;j >=0;j--)
	   SendByte(a[j]);
}



/*----------------------中断------------------------------*/
void Uart_SerialPort() interrupt 4 
{
	if(RI)
	{
		EA=0;  //关闭中断
		RI=0;
  		info=SBUF;
		UART_bit9=RB8;
		rbusy=true;
  	} 	 
  if(TI)
  {
  		EA=0;  //关闭中断
  		TI=0;
		busy=false;  //串口繁忙
	} 
	EA=1;  //开启中断
}


