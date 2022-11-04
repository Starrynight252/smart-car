#ifndef _VEHICLE_H_
#define _VEHICLE_H_

/*#################����ͷ�ļ�#######################*/
#include"project.h"

/*#################���ñ���#######################*/
extern uchar info;	   //���ڻ���
extern Front_bool busy,rbusy;  //����״̬
/*#################����/�궨������#######################*/
/*******��״̬����*********/
#define LAMP_ON	 0
#define LAMP_OFF  1
#define P1_INIT 0xFF    //0X80     //0x00
#define LAMP_P2_INIT 0xFF    //0X80     //0x00
/*******���״̬����********/
#define SE_TIMINGLNITIAL 10      //�����ʼ��ʱ
#define SE_RANGEMAX 17      //�����Χ
#define SE_RANGEMIN 0      //�����Χ
#define SE_TIMINGMAX 30			  //�����ʱ����
/*******��ﶨ��*********/
#define MOTOR_TIMINGMAX 200      //����ʱ����
/*******�����С����*********/
#define ARRAY_SIZE 10 //�����С

/*###########################����ԭ��#######################*/

/************************************
*-����: Car_Initi                   *
*-��������: ������ʼ��              *
*-���: �����豸,ģʽ,�ٶȳ�ʼ��,	*
* B_Horn��80us�仯,���ö�ʱ��0		*
*-�޲���                            *
*-�޷���ֵ			                */
void Car_Initi();
/************************************
*-����: Vehicle_Stop                *
*-��������: ֹͣ,ͣ��               *
*-���: ��ɲ����,EA�仯һ��	    *
*  EM_OUT1-4Ϊ1*					*
*-�޲���                            *
*-�޷���ֵ			                */
void Vehicle_Stop();
/************************************
*-����: Vehicle_ForwardRotation     *
*-��������: ��ת                    *
*-���: ɲ��������,EM_OUT2��4Ϊ0	*
*-�޲���                            *
*-�޷���ֵ			                */
void Vehicle_ForwardRotation();
/************************************
*-����: Vehicle_Reverse             *
*-��������: ��ת                    *
*-���: ����,EM_OUT1��3Ϊ1          *					
* �ر�ɲ����					    *
*-�޲���                            *
*-�޷���ֵ			                */
void Vehicle_Reverse();
/***************************
*-����: Cornering_Lamp     *
*-��������: ����ת���     *
*-���: ����ת���         *					
*-�޲���                   * 
*-�޷���ֵ			       */
void Parameter_Steering(const uchar direction);
/****************************************************
*-����: Parameter_Steering                          *
*-��������: ��ʼλ��,��ת,��ת                      *
*-���: ��+���ת��(--/++/==SE_PwmCount),����	    *
*   Cornering_Lamp����  							*					
*-����:                             				*
*#uchar direction ת����(L,R,O) ��ת,��ת,��ʼλ��*  
*-�޷���ֵ			                                */
void Parameter_Steering(uchar direction);
/**********************************************
*-����: VehicleTurnLeft                       *
*-��������: ת�򱣳�ʱ��                      *
*-���: �����Բ���directionת��,Ȼ�󱣳ֲ���  *
* keepus(��λ:us)��,�ָ�{ע:�෴����ת��һ��} *					
*-����:const								  *
*#uchar direction  ת����(L,R)			  *
*#uint keepus      ����ʱ��/us                *
*#uint angle	   ת��Ƕ�0�������Χ:       *
* ��ʼλ��Ϊ0 ��������:(MIN<-��ʼ0λ��->MAX)  *
*˵���˾���ִ�м���                           *
*-Front_bool���ͷ���ֵ-�ɹ�ʧ��			      */
Front_bool Steering_HoldingTime(const uchar direction,const uint keepus,const uint angle);

/**********************************************
*-����: Stop_SettingSpeed_Parameters   		  *
*-��������: ͣ�������ٶȲ���                  *
*-���:ͣ��,ʹ��Modernspeed���������ٶȲ���	  *
* ����ԭ�����ٶ�						      *
*-����:										  *
*#uint Modernspeed  ���õ��ٶ�		     	  *
*-uint���ͷ���ֵ-ԭ���ٶ�(uint)			      */
uint Stop_SettingSpeed_Parameters(uint Modernspeed);
/**********************************************
*-����: Speed_Value                           *
*-��������: ͣ�������ٶȲ���                  *
*-���: ����Get_InputNumber_UintType��ȡ�ٶ�ֵ*
* �������û�ȡ���ٶ�,				          *
*-�޲���                                      * 
*-Front_bool���ͷ���ֵ-�ɹ�ʧ��			      */
Front_bool Speed_Value();
/**********************************************
*-����: SpeedMode_Din                         *
*-��������: ����������                        *
*-���: ���ô󲿷ֳ�������,���ݲ��������ٶȵ�,*				          
*-����:										  *
*#uchar command,���������                    * 
*-Front_bool���ͷ���ֵ-�ɹ�/ʧ��		      */
Front_bool SpeedMode_Din(uchar command);
/************************************
*-����: Option_prompt               *
*-��������: ����ѡ����ʾ            *
*-���: ��ӡ�ٶȺͷ������       	*
*-�޲���                            *
*-�޷���ֵ			                */
void Option_prompt();
/************************************
*-����: CommandProcessing           *
*-��������: �ַ�������              *
*-���: ��ȡ����ARRAY_SIZE������� 	*
* ������ʱִ��						*
*-�޲���                            *
*-Front_bool���ͷ���ֵ-�ɹ�/ʧ��    */
Front_bool CommandProcessing();

/****************��ɫ����->Ѱ�� Black tracking****************/
/*###########################����ԭ��#######################*/

/****************************************
*-����: Black_Tracking_Action         	*
*-��������: Ѱ���߼����ƶ�����          *
*-���: ��⳵���Ƿ��ڹ켣����,��:		*
* ����,ת��ֹͣ������'B'�˳�����		*
*    ��������							*
����  *								    *
*-�޲���                                *
*-�޷���ֵ			                    */
void Black_Tracking_Action();

#endif