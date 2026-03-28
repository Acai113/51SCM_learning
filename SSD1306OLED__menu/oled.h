#ifndef __OLED_H__
#define __OLED_H__
#include<REGX52.H>

sbit OLED_SCL = P1^0;  //时钟
sbit OLED_SDA = P1^1;  //数据
sbit OLED_RES = P1^2;  //复位
sbit OLED_DC = P1^3;  //数据/命令选择
sbit OLED_CS = P1^4; //片选

void Delay(unsigned int xms);
void OLED_Write_SPI(unsigned char dat); /*通过SPI发送一个字节*/
void OLED_Write_Cmd(unsigned char cmd); //写入命令
void OLED_Write_Data(unsigned char dat); //写入数据

void OLED_Init(void); //初始化
void OLED_Clear(void); //清屏函数



#endif