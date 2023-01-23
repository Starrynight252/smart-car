/*
*说明：_EEPROM_的代码大部分
*根据STC官网提供的STC89C52xx手册改写
*/
#include"EEPROM.h"

/*----------------------------
Software delay function
----------------------------*/
void Delay(uchar n)
{
 uchar x;
 while (n--)
 {
 x = 0;
 while (++x);
 }
 }


 
 /*----------------------------
禁用ISP/IAP/EEPROM功能

使MCU处于安全状态
----------------------------*/
void IapIdle()
{
 IAP_CONTR = 0; //Close IAP function
 IAP_CMD = 0; //Clear command to standby
 IAP_TRIG = 0; //Clear trigger register
 IAP_ADDRH = 0x80; //Data ptr point to non-EEPROM area
 IAP_ADDRL = 0; //Clear IAP address to prevent misuse
}
/*----------------------------
从ISP/IAP/EEPROM区域读取一个字节
输入：地址（ISP/IAP/EEPROM地址）
输出：闪存数据
----------------------------*/
uchar IapReadByte(uint addr)
{
 uchar dat; //Data buffer
 IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
 IAP_CMD = CMD_READ; //Set ISP/IAP/EEPROM READ command
 IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
 IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
 IAP_TRIG = 0x46; //Send trigger command1 (0x46)
 IAP_TRIG = 0xb9; //Send trigger command2 (0xb9)
 _nop_(); //MCU will hold here until ISP/IAP/EEPROM operation complete
 dat = IAP_DATA; //Read ISP/IAP/EEPROM data
 IapIdle(); //Close ISP/IAP/EEPROM function
 return dat; //Return Flash data
}
/*----------------------------
从ISP/IAP/EEPROM区域读取一个字节
输入：地址（ISP/IAP/EEPROM地址）
输出：闪存数据
----------------------------*/
uint IapReadNum(uint addr)
{
 uchar dat; //Data buffer
 IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
 IAP_CMD = CMD_READ; //Set ISP/IAP/EEPROM READ command
 IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
 IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
 IAP_TRIG = 0x46; //Send trigger command1 (0x46)
 IAP_TRIG = 0xb9; //Send trigger command2 (0xb9)
 _nop_(); //MCU will hold here until ISP/IAP/EEPROM operation complete
 dat = IAP_DATA; //Read ISP/IAP/EEPROM data
 IapIdle(); //Close ISP/IAP/EEPROM function
 return dat%48; //Return Flash data
}
/*----------------------------
将一个字节编程到ISP/IAP/EEPROM区域
输入：地址（ISP/IAP/EEPROM地址）
dat（ISP/IAP/EEPROM数据）
输出：-
----------------------------*/
void IapProgramByte(uint addr,uchar dat)
{
 IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
 IAP_CMD = CMD_PROGRAM; //Set ISP/IAP/EEPROM PROGRAM command
 IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
 IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
 IAP_DATA = dat; //Write ISP/IAP/EEPROM data
 IAP_TRIG = 0x46; //Send trigger command1 (0x46)
 IAP_TRIG = 0xb9; //Send trigger command2 (0xb9)
 _nop_(); //MCU will hold here until ISP/IAP/EEPROM operation complete
 IapIdle();
}
/*----------------------------
删除一个扇区区域
输入：地址（ISP/IAP/EEPROM地址）
输出：-
----------------------------*/
void IapEraseSector(uint addr)
{
 IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
 IAP_CMD = CMD_ERASE; //Set ISP/IAP/EEPROM ERASE command
 IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
 IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
 IAP_TRIG = 0x46; //Send trigger command1 (0x46)
 IAP_TRIG = 0xb9; //Send trigger command2 (0xb9)
 _nop_(); //MCU will hold here until ISP/IAP/EEPROM operation complete
 IapIdle();
 }


