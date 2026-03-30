#ifndef __OLED_H__
#define __OLED_H__
#include<REGX52.H>

#define  u8 unsigned char 
#define  u16 unsigned int
#define  u32 unsigned int

#define OLED_CMD 0 //0=命令
#define OLED_DATA 1 //1=数据

sbit OLED_SCL = P1^0;  //时钟
sbit OLED_SDA = P1^1;  //数据
sbit OLED_RES = P1^2;  //复位
sbit OLED_DC = P1^3;  //数据/命令选择
sbit OLED_CS = P1^4; //片选

/*端口定义    Clr=0,Set=1*/
#define OLED_SCL_Clr() OLED_SCL=0
#define OLED_SCL_Set() OLED_SCL=1

#define OLED_SDA_Clr() OLED_SDA=0
#define OLED_SDA_Set() OLED_SDA=1

#define OLED_RES_Clr() OLED_RES=0
#define OLED_RES_Set() OLED_RES=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

void delay_ms(u32 xms);
void OLED_WR_Byte(u8 dat, u8 cmd); /*通过SPI发送一个字节,数据或命令*/
void OLED_Set_Pos(u8 x, u8 y);  //设置显示坐标
void OLED_Set_Simple(u8 x, u8 y);  //显示一个像素点

void OLED_Init(void); //初始化
void OLED_Clear(void); //清屏函数
void OLED_ShowChar(u8 x. u8 y, u8 chr, u8 sizey); //显示字符 列坐标，页坐标，字符，大小
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey); //显示数字 列坐标，页坐标，数字，长度，大小
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey);  //显示字符串 列坐标，页坐标，字符串，大小
void OLED_ShowChinese(u8 x,u8 y,u8 no,u8 sizey)  //显示汉字 列坐标，页坐标，下标，大小


#endif