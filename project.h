#ifndef _POROJECT_H_
#define _POROJECT_H_

/*#################����ͷ�ļ�#######################*/
#include<reg52.h>
#include<intrins.h>
#include<stdio.h>
#include<stdlib.h>

/*###########################typedef ��������#######################*/
typedef enum{false=0,true,blck}Front_bool;
/*ע:���ر�˵��,��������Front_bool����
*��true Ϊ�ɹ�(��),falseΪʧ��(��)   */	
typedef unsigned char uchar;
typedef unsigned int  uint;

/*###########################��λ����#######################*/
//p0 
/*p0.0-p0.3 Ϊ��ɫ����->Ѱ��*/
sbit Front_MiddleTracking=P0^0;	  //ǰ����
sbit FrontL_MiddleTracking=P0^1; //ǰ����
sbit FrontR_MiddleTracking=P0^2; //ǰ����
sbit Rear_MiddleTracking=P0^3;	  //��

sbit Retai_zeron=P0^4;	  //������0
sbit Retain_One=P0^5;	  //������
sbit Retain_two=P0^6;	   //������
sbit Retain_three=P0^7;	   //������

//P1 �Զ���
sbit Custom_zero=P1^0;	//0
sbit Custom_One=P1^1;	//1
sbit Custom_two=P1^2;	//2			    
sbit Custom_three=P1^3; //3
sbit Custom_four=P1^4;   //4 	   
sbit Custom_five=P1^5;  //5
sbit Custom_six=P1^6;  
sbit Custom_seven=P1^7; 
  
//p2						   
sbit Left_Beam=P2^0;	     //��
sbit Right_Beam=P2^1;	//��			    
sbit Inverse_Beam=P2^2; //����
sbit Brake_Beam=P2^3;   //ɲ�� 	   
sbit Low_Beam=P2^4;  //�����
sbit High_Beam=P2^5;  //Զ���
sbit Peripheral=P2^6;  //��Χ�豸
sbit R_Horn=P2^7;  //����-����

					 
//P3  ����
sbit UART_RXD=P3^0;
sbit UART_TXD=P3^1;
sbit UART_bit9=P3^2;
//�������
sbit EM_OUT1=P3^3; 
sbit EM_OUT2=P3^4;             
sbit EM_OUT3=P3^5;           
sbit EM_OUT4=P3^6;
sbit SE_OUT1=P3^7;

#endif