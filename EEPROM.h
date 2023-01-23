/*
*˵����_EEPROM_�Ĵ���󲿷�
*����STC�����ṩ��STC89C52xx�ֲ��д
*/
#ifndef _EEPROM_H_
#define _EEPROM_H_

#include"project.h"

/*Declare SFR associated with the IAP */
sfr IAP_DATA = 0xE2; //Flash data register
sfr IAP_ADDRH = 0xE3; //Flash address HIGH
sfr IAP_ADDRL = 0xE4; //Flash address LOW
sfr IAP_CMD = 0xE5; //Flash command register
sfr IAP_TRIG = 0xE6; //Flash command trigger
sfr IAP_CONTR = 0xE7; //Flash control register
/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE 0 //Stand-By
#define CMD_READ 1 //Byte-Read
#define CMD_PROGRAM 2 //Byte-Program
#define CMD_ERASE 3 //Sector-Erase
/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
//#define ENABLE_IAP 0x80 //if SYSCLK<40MHz
//#define ENABLE_IAP 0x81 //if SYSCLK<20MHz
#define ENABLE_IAP 0x82 //if SYSCLK<10MHz
//#define ENABLE_IAP 0x83 //if SYSCLK<5MHz
 //EEPROM ��ʼ��ַ
#define IAP_ADDRESS 0x2000

void Delay(uchar n);
 /*----------------------------
����ISP/IAP/EEPROM����

ʹMCU���ڰ�ȫ״̬
----------------------------*/
void IapIdle();
/*----------------------------
��ISP/IAP/EEPROM�����ȡһ���ֽ�
���룺��ַ��ISP/IAP/EEPROM��ַ��
�����uchar��������
----------------------------*/
uchar IapReadByte(uint addr);
/*----------------------------
��ISP/IAP/EEPROM�����ȡһ���ֽ�
���룺��ַ��ISP/IAP/EEPROM��ַ��
�����uint��������
----------------------------*/
uint IapReadNum(uint addr);
/*----------------------------
��һ���ֽڱ�̵�ISP/IAP/EEPROM����
���룺��ַ��ISP/IAP/EEPROM��ַ��
dat��ISP/IAP/EEPROM���ݣ�
�����-
----------------------------*/
void IapProgramByte(uint addr,uchar dat);
/*----------------------------
ɾ��һ����������
���룺��ַ��ISP/IAP/EEPROM��ַ��
�����-
----------------------------*/
void IapEraseSector(uint addr);

#endif