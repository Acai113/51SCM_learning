#include"oled.h"
#include<INTRINS.H>

void Delay(unsigned int xms)	//延时函数@11.0592MHz
{
	unsigned char data i, j;
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}
/*通过SPI发送一个字节*/
void OLED_Write_SPI(unsigned char dat)
{
	unsigned char i;
	
	for(i = 0; i < 8; i++)
	{
		OLED_SCL = 0;
		if(dat &0x80) OLED_SDA = 1;
		else OLED_SDA = 0;
		OLED_SCL = 1;  //上拉读取
		dat <<=1;
	}
	OLED_SCL = 0;
}

/*向OLED写入命令*/
void OLED_Write_Cmd(unsigned char cmd)
{
	OLED_DC = 0;  //表示命令
	OLED_CS = 0;  //选中
	OLED_Write_SPI(cmd);  //写入命令
	OLED_CS = 1;  //释放选片
}

/*向OLED写入数据*/
void OLED_Write_Data(unsigned char dat)
{
	OLED_DC = 1; //表示数据
	OLED_CS = 0;
	OLED_Write_SPI(dat);  //写入数据
	OLED_CS = 1;  //释放选片
}

void OLED_Init(void)
{
	//硬件复位
	OLED_RES = 0; 
	Delay(100);
	OLED_RES = 1;
	Delay(100);
	
	//初始化命令序列
	OLED_Write_Cmd(0xAE); //关闭显示
	
	OLED_Write_Cmd(0xD5); //设置时钟分频
	OLED_Write_Cmd(0x80); 
	
	OLED_Write_Cmd(0xA8); //设置复用比
	OLED_Write_Cmd(0x3F); 
	
	OLED_Write_Cmd(0xD3); //设置显示偏移
	OLED_Write_Cmd(0x00); 
	
	OLED_Write_Cmd(0x40); //设置显示起始行
	
	OLED_Write_Cmd(0x8D); //电荷泵设置
	OLED_Write_Cmd(0x14); //开启电荷泵
	
	OLED_Write_Cmd(0xA1); //段重映射
	OLED_Write_Cmd(0xC8); //COM输出方向反转
	
	OLED_Write_Cmd(0xDA); //COM引脚配置
	OLED_Write_Cmd(0x12); 
	
	OLED_Write_Cmd(0x81); //对比度
	OLED_Write_Cmd(0xCF);

	OLED_Write_Cmd(0xD9); //预充电周期
	OLED_Write_Cmd(0xF1);

	OLED_Write_Cmd(0xDB); //VCOMH电平
	OLED_Write_Cmd(0x40);

	OLED_Write_Cmd(0xA4); //正常显示
	OLED_Write_Cmd(0xA6); //非反色
	
	OLED_Write_Cmd(0xAF); //开启显示
}

void OLED_Clear(void)
{
	unsigned char page, col;
	
	for(page = 0; page < 8; page++)
	{
		OLED_Write_Cmd(0xB0 + page); //设置页地址
		OLED_Write_Cmd(0x00); //列低4位
		OLED_Write_Cmd(0x10); //列高4位
		
		for(col = 0; col < 128; col++)
		{
			OLED_Write_Data(0x00); //写入0
		}
	}
}