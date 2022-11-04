#ifndef _UART_H_
#define _UART_H_

/*#################引用头文件#######################*/
#include"project.h"

/*#################常量/宏定义声明#######################*/
/*******************************************************************************
*FOSC		为外部晶振	12000000L为12MHz	11059200L==11.0592MHz
*BAUD 		为波特率
*PARITYBIT  奇偶校验位 0-无  1-奇校验位 2-偶校验位  3-标记校验位  4-空间校验位 	   
********************************************************************************/
#define FOSC 11059200L//12000000L //11059200L     
#define BAUD  9600 //9600
#define PARITYBIT 0

/*###########################函数原型#######################*/
/**********************
*-名称:delay_nus  	  *
*-函数功能: 延时      *
*-参数:				  *
*#uint time 以us为单位*
*-注:使用_nop_()	  *
*-无返回值			  */
void delay_nus(uint time);
/**********************
*-名称:delay_ms  	  *
*-函数功能: 延时      *
*-参数:				  *
*#uint time  ms为单位 *
*-无返回值			  */
void delay_ms(uint time);
/***************************
*-名称:UART_init           *
*-函数功能: UART初始化     *
*-后果: 设置定时器1工作    *
* 在模式2作为串口通讯      *
* 并且设置优先级           *
*-注:可以更改宏改变波特率等*
*-无参数                   *
*-无返回值			       */
void UART_init();
/********************************
*-名称:End_Flag                 *
*-函数功能: 从UART读入结束      *
* 标志ASCII(0x0D) 回车          *
*-注:可以更改if改变判定         *
*只有返回假时rbusy不置位       *
*-无参数                        *
*- Front_bool类型返回值-成功失败*/
Front_bool End_Flag();
/***********************************************
*-名称: Get_InputString_UcharType              *
*-函数功能: 获取输入字符串-uchar类型           *
*-后果: 从uart读取;							   *
* 结束:1如果输入达到数组大小,则结束读入并且返回*
*      2.如果输入回车,则结束读入并且返回	   *					  *
*-参数:										   *
*#uchar *array  uchar类型数组			       *
*#const uint length   数组长度                 *
*-uint类型返回值-数组保存的长度			       */
uint Get_InputString_UcharType(uchar *array,const uint length);
/***********************************************
*-名称: Get_InputNumber_UintType               *
*-函数功能: 获取输入数值-int类型              *
*-后果: 从uart读取;							   *
* 结束:1如果输入达到数组大小,则结束读入并且返回*
*      2.如果输入回车,则结束读入并且返回	   *					  *
*-参数:										   *
*#uint *array  uint类型数组			       *
*#const uint length   数组长度                 *
*-uint类型返回值-数组保存的长度			       */
uint Get_InputNumber_UintType(uint *array,const uint length);
/******************************
*-名称:SendByte               *
*-函数功能: 发送1字节(1Byte)  *
*-参数:						  *
*#const uchar date 待发送1字节*
*-无返回值					  */
void SendByte(const uchar date);

/***************************************
*-名称:SendString                      *
*-函数功能: 发送字符串                 *
*-参数:date待发送1次1字节类型:(uchar*) *
*-无返回值			                   */
void SendString(const uchar* date);

/*************************************
*-名称:SendNum                       *
*-函数功能: 发送整(int)数            *
*-参数:date待发送1次1字节类型:uint 	 *
*-无返回值			                 */
void SendNum(uint i);

#endif