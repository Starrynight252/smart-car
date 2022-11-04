#ifndef _POROJECT_H_
#define _POROJECT_H_

/*#################引用头文件#######################*/
#include<reg52.h>
#include<intrins.h>
#include<stdio.h>
#include<stdlib.h>

/*###########################typedef 类型声明#######################*/
typedef enum{false=0,true,blck}Front_bool;
/*注:无特别说明,函数返回Front_bool类型
*则true 为成功(真),false为失败(假)   */	
typedef unsigned char uchar;
typedef unsigned int  uint;

/*###########################脚位定义#######################*/
//p0 
/*p0.0-p0.3 为黑色跟踪->寻迹*/
sbit Front_MiddleTracking=P0^0;	  //前方中
sbit FrontL_MiddleTracking=P0^1; //前方左
sbit FrontR_MiddleTracking=P0^2; //前方右
sbit Rear_MiddleTracking=P0^3;	  //后方

sbit Retai_zeron=P0^4;	  //保留口0
sbit Retain_One=P0^5;	  //保留口
sbit Retain_two=P0^6;	   //保留口
sbit Retain_three=P0^7;	   //保留口

//P1 自定义
sbit Custom_zero=P1^0;	//0
sbit Custom_One=P1^1;	//1
sbit Custom_two=P1^2;	//2			    
sbit Custom_three=P1^3; //3
sbit Custom_four=P1^4;   //4 	   
sbit Custom_five=P1^5;  //5
sbit Custom_six=P1^6;  
sbit Custom_seven=P1^7; 
  
//p2						   
sbit Left_Beam=P2^0;	     //左
sbit Right_Beam=P2^1;	//右			    
sbit Inverse_Beam=P2^2; //倒车
sbit Brake_Beam=P2^3;   //刹车 	   
sbit Low_Beam=P2^4;  //近光灯
sbit High_Beam=P2^5;  //远光灯
sbit Peripheral=P2^6;  //外围设备
sbit R_Horn=P2^7;  //喇叭-提醒

					 
//P3  串口
sbit UART_RXD=P3^0;
sbit UART_TXD=P3^1;
sbit UART_bit9=P3^2;
//电机控制
sbit EM_OUT1=P3^3; 
sbit EM_OUT2=P3^4;             
sbit EM_OUT3=P3^5;           
sbit EM_OUT4=P3^6;
sbit SE_OUT1=P3^7;

#endif