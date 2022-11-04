#include"UART.h"

/*#################�������#######################*/
Front_bool busy,rbusy;  //����״̬
uchar info='*';	   //���ڻ���

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
		for(b=110;b>0;b--); //��ʱ
}
/**************UART************************/
void UART_init()
{
	rbusy=false;
	TMOD|=0x20; //���ö�ʱ��1������ģʽ2��Ϊ����ͨѶ
	/*RCAP2L =TL2=(UCHAR_MAX-(FOSC/32/BAUD));		//UART
	//RCAP2H =TH2=(UCHAR_MAX-(FOSC/32/BAUD))>>8;
	//PT1=1;	
	T2CON=0x34;
	//PT0=0;
	*/
	IP=0x10;
	//����STC�����ṩ�Ĳ����ʼ��㹫ʽ
	TH1=TL1=-(FOSC/12/32/BAUD); 
	 
	#if(PARITYBIT==0)
		SCON=0X50; 	
	#elif(PARITYBIT==1)
		SCON=0Xda;
	#elif(PARITYBIT==2)
		SCON=0Xd2; 
	#endif

	TR1=1;//����T1  
	
	ES=1; //��Ӧ�ж�
	EA=1; //�����ж�
}
//������־
Front_bool End_Flag()
{	
	if(info==0x0D){//�س�
		rbusy=false;
		return true;
	}
	return false;
}
/***************�����ֽ�********************************/
//1.ʹ��rbusy�ж�,trueΪ�Ѿ����1�ֽ�����,�෴falseΪû�л��1�ֽ�����
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
	/*��Ϊ��֧��VLA(C99),�����޷���ʹ��Get_InputString_UcharType()
	��Ϊ����Ҫ������Ԫ�س����޷�ȷ��.*/
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
/***************�����ֽ�********************************/
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
//�����ַ���
void SendString(const uchar* date)
{
	while(*date)
	 SendByte(*date++);	
//	SendByte('\0');	
}
//������(int)��
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



/*----------------------�ж�------------------------------*/
void Uart_SerialPort() interrupt 4 
{
	if(RI)
	{
		EA=0;  //�ر��ж�
		RI=0;
  		info=SBUF;
		UART_bit9=RB8;
		rbusy=true;
  	} 	 
  if(TI)
  {
  		EA=0;  //�ر��ж�
  		TI=0;
		busy=false;  //���ڷ�æ
	} 
	EA=1;  //�����ж�
}


