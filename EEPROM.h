/*
*说明：_EEPROM_的代码大部分
*根据STC官网提供的STC89C52xx手册改写
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
 //EEPROM 起始地址
#define IAP_ADDRESS 0x2000

void Delay(uchar n);
 /*----------------------------
禁用ISP/IAP/EEPROM功能

使MCU处于安全状态
----------------------------*/
void IapIdle();
/*----------------------------
从ISP/IAP/EEPROM区域读取一个字节
输入：地址（ISP/IAP/EEPROM地址）
输出：uchar闪存数据
----------------------------*/
uchar IapReadByte(uint addr);
/*----------------------------
从ISP/IAP/EEPROM区域读取一个字节
输入：地址（ISP/IAP/EEPROM地址）
输出：uint闪存数据
----------------------------*/
uint IapReadNum(uint addr);
/*----------------------------
将一个字节编程到ISP/IAP/EEPROM区域
输入：地址（ISP/IAP/EEPROM地址）
dat（ISP/IAP/EEPROM数据）
输出：-
----------------------------*/
void IapProgramByte(uint addr,uchar dat);
/*----------------------------
删除一个扇区区域
输入：地址（ISP/IAP/EEPROM地址）
输出：-
----------------------------*/
void IapEraseSector(uint addr);

#endif