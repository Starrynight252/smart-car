#ifndef _VEHICLE_H_
#define _VEHICLE_H_

/*#################引用头文件#######################*/
#include"project.h"

/*#################引用变量#######################*/
extern uchar info;	   //串口缓存
extern Front_bool busy,rbusy;  //串口状态
/*#################常量/宏定义声明#######################*/
/*******灯状态定义*********/
#define LAMP_ON	 0
#define LAMP_OFF  1
#define P1_INIT 0xFF    //0X80     //0x00
#define LAMP_P2_INIT 0xFF    //0X80     //0x00
/*******舵机状态定义********/
#define SE_TIMINGLNITIAL 10      //舵机初始计时
#define SE_RANGEMAX 17      //舵机范围
#define SE_RANGEMIN 0      //舵机范围
#define SE_TIMINGMAX 30			  //舵机计时上限
/*******马达定义*********/
#define MOTOR_TIMINGMAX 200      //马达计时上限
/*******数组大小定义*********/
#define ARRAY_SIZE 10 //数组大小

/*###########################函数原型#######################*/

/************************************
*-名称: Car_Initi                   *
*-函数功能: 车辆初始化              *
*-后果: 外置设备,模式,速度初始化,	*
* B_Horn口80us变化,设置定时器0		*
*-无参数                            *
*-无返回值			                */
void Car_Initi();
/************************************
*-名称: Vehicle_Stop                *
*-函数功能: 停止,停车               *
*-后果: 打开刹车灯,EA变化一次	    *
*  EM_OUT1-4为1*					*
*-无参数                            *
*-无返回值			                */
void Vehicle_Stop();
/************************************
*-名称: Vehicle_ForwardRotation     *
*-函数功能: 正转                    *
*-后果: 刹车倒车灯,EM_OUT2和4为0	*
*-无参数                            *
*-无返回值			                */
void Vehicle_ForwardRotation();
/************************************
*-名称: Vehicle_Reverse             *
*-函数功能: 反转                    *
*-后果: 倒车,EM_OUT1和3为1          *					
* 关闭刹车灯					    *
*-无参数                            *
*-无返回值			                */
void Vehicle_Reverse();
/***************************
*-名称: Cornering_Lamp     *
*-函数功能: 控制转向灯     *
*-后果: 控制转向灯         *					
*-无参数                   * 
*-无返回值			       */
void Parameter_Steering(const uchar direction);
/****************************************************
*-名称: Parameter_Steering                          *
*-函数功能: 初始位置,右转,左转                      *
*-后果: 灯+舵机转向(--/++/==SE_PwmCount),调用	    *
*   Cornering_Lamp函数  							*					
*-参数:                             				*
*#uchar direction 转向方向(L,R,O) 右转,左转,初始位置*  
*-无返回值			                                */
void Parameter_Steering(uchar direction);
/**********************************************
*-名称: VehicleTurnLeft                       *
*-函数功能: 转向保持时间                      *
*-后果: 首先以参数direction转向,然后保持参数  *
* keepus(单位:us)后,恢复{注:相反方向转动一次} *					
*-参数:const								  *
*#uchar direction  转向方向(L,R)			  *
*#uint keepus      保持时间/us                *
*#uint angle	   转向角度0到舵机范围:       *
* 初始位置为0 左右区间:(MIN<-初始0位置->MAX)  *
*说白了就是执行几次                           *
*-Front_bool类型返回值-成功失败			      */
Front_bool Steering_HoldingTime(const uchar direction,const uint keepus,const uint angle);

/**********************************************
*-名称: Stop_SettingSpeed_Parameters   		  *
*-函数功能: 停车设置速度参数                  *
*-后果:停车,使用Modernspeed参数设置速度并且	  *
* 返回原来的速度						      *
*-参数:										  *
*#uint Modernspeed  设置的速度		     	  *
*-uint类型返回值-原来速度(uint)			      */
uint Stop_SettingSpeed_Parameters(uint Modernspeed);
/**********************************************
*-名称: Speed_Value                           *
*-函数功能: 停车设置速度参数                  *
*-后果: 调用Get_InputNumber_UintType获取速度值*
* 并且设置获取的速度,				          *
*-无参数                                      * 
*-Front_bool类型返回值-成功失败			      */
Front_bool Speed_Value();
/**********************************************
*-名称: SpeedMode_Din                         *
*-函数功能: 车参数设置                        *
*-后果: 调用大部分车辆函数,根据参数设置速度等,*				          
*-参数:										  *
*#uchar command,传入的命令                    * 
*-Front_bool类型返回值-成功/失败		      */
Front_bool SpeedMode_Din(uchar command);
/************************************
*-名称: Option_prompt               *
*-函数功能: 命令选项提示            *
*-后果: 打印速度和方向参数       	*
*-无参数                            *
*-无返回值			                */
void Option_prompt();
/************************************
*-名称: CommandProcessing           *
*-函数功能: 字符串命令              *
*-后果: 获取输入ARRAY_SIZE宏个命令 	*
* 并且延时执行						*
*-无参数                            *
*-Front_bool类型返回值-成功/失败    */
Front_bool CommandProcessing();

/****************黑色跟踪->寻迹 Black tracking****************/
/*###########################函数原型#######################*/

/****************************************
*-名称: Black_Tracking_Action         	*
*-函数功能: 寻迹逻辑的移动函数          *
*-后果: 检测车辆是否在轨迹线上,在:		*
* 运行,转向，停止，输入'B'退出函数		*
*    串口命令							*
优先  *								    *
*-无参数                                *
*-无返回值			                    */
void Black_Tracking_Action();

#endif