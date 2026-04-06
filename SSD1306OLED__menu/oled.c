#include"oled.h"
#include"oledfont.h"
#include<INTRINS.H>

void delay_ms(unsigned int xms)	//延时函数@11.0592MHz
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
void OLED_WR_Byte(u8 dat, u8 cmd)
{
	u8 i;
	if(cmd) OLED_DC_Set();  //cmd=1,表示数据
	else OLED_DC_Clr();  //cmd=0,表示命令
	
	OLED_CS_Clr();
	for(i = 0; i < 8; i++)
	{
		OLED_SCL_Clr();
		if(dat & 0x80) OLED_SDA_Set();
		else OLED_SDA_Clr();
		OLED_SCL_Set();  //上拉读取
		dat <<= 1;
	}
	OLED_CS_Set();
}

//坐标设置
void OLED_Set_Pos(u8 x, u8 y)
{
	OLED_WR_Byte(0xB0+y, OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10, OLED_CMD);
	OLED_WR_Byte((x&0x0f), OLED_CMD);
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void OLED_Init(void)
{
	//硬件复位
	OLED_RES_Clr(); 
	delay_ms(100);
	OLED_RES_Set();
	delay_ms(100);
	
	//初始化命令序列
	OLED_WR_Byte(0xAE, OLED_CMD); //关闭显示
	
	OLED_WR_Byte(0xD5, OLED_CMD); //设置时钟分频
	OLED_WR_Byte(0x80, OLED_CMD); 
	
	OLED_WR_Byte(0xA8, OLED_CMD); //设置复用比
	OLED_WR_Byte(0x3F, OLED_CMD); 
	
	OLED_WR_Byte(0xD3, OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0x00, OLED_CMD); 
	
	OLED_WR_Byte(0x40, OLED_CMD); //设置显示起始行
	
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14, OLED_CMD); //开启电荷泵
	
	OLED_WR_Byte(0xA1, OLED_CMD); //段重映射
	OLED_WR_Byte(0xC8, OLED_CMD); //COM输出方向反转
	
	OLED_WR_Byte(0xDA, OLED_CMD); //COM引脚配置
	OLED_WR_Byte(0x12, OLED_CMD); 
	
	OLED_WR_Byte(0x81, OLED_CMD); //对比度
	OLED_WR_Byte(0xCF, OLED_CMD);

	OLED_WR_Byte(0xD9, OLED_CMD); //预充电周期
	OLED_WR_Byte(0xF1, OLED_CMD);

	OLED_WR_Byte(0xDB, OLED_CMD); //VCOMH电平
	OLED_WR_Byte(0x40, OLED_CMD);

	OLED_WR_Byte(0xA4, OLED_CMD); //正常显示
	OLED_WR_Byte(0xA6, OLED_CMD); //非反色
	
	OLED_WR_Byte(0xAF, OLED_CMD); //开启显示
}

void OLED_Clear(void)
{
	u8 page, col;
	
	for(page = 0; page < 8; page++)
	{
		OLED_WR_Byte(0xB0 + page, OLED_CMD); //设置页地址
		OLED_WR_Byte(0x00, OLED_CMD); //列低4位
		OLED_WR_Byte(0x10, OLED_CMD); //列高4位
		
		for(col = 0; col < 128; col++)
		{
			OLED_WR_Byte(0x00, OLED_DATA); //写入0
		}
	}
}

/*显示一个像素点*/
void OLED_Set_Simple(unsigned char x, unsigned char y)
{
	u8 page;
	u8 bit_mask;
	
	page = y/8;
	bit_mask = 1<<(y%8);
	
	OLED_WR_Byte(0xB0 + page, OLED_CMD); //
	OLED_WR_Byte(x & 0x0F, OLED_CMD); //
	OLED_WR_Byte((0x10 | (x >> 4)), OLED_CMD); //
	OLED_WR_Byte(bit_mask, OLED_DATA); //
}

/*显示一个字符*/
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 sizey)
{
	u8 c=0, sizex=sizey/2;
	u16 i, size1;  //size1为字符点阵数据的总字节数
	if(sizey==8) size1=6;
	else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
	c = chr - ' ';
	OLED_Set_Pos(x,y); 
	for(i=0;i<size1;i++)  //列地址自动递增，只需要改变页地址
	{
		if(i%sizex==0&&sizey!=8) OLED_Set_Pos(x,y++);
		if(sizey==8) OLED_WR_Byte(asc2_0806[c][i],OLED_DATA);//6X8字号
		else if(sizey==16) OLED_WR_Byte(asc2_1608[c][i],OLED_DATA);//8x16字号
		
		else return;
	}
}

/*显示数字，可多位*/
void OLED_ShowNum(u8 x,u8 y,u32 num, u8 sizey)
{         	
	u8 t,temp,m=0;
	u8 enshow=0;
	
	u32 n = num;
	u8 len = 0;
	
	if(sizey==8)m=2;
	
	if(n==0) len=1;
	else
	{
		while(n>0)
		{
			n = n/10;
			len++;
		}
	}
	
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(sizey/2+m)*t,y,' ',sizey);
				continue;
			}else enshow=1;
		}
	 	OLED_ShowChar(x+(sizey/2+m)*t,y,temp+'0',sizey);
	}
}

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey)
{
	u8 j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j++],sizey);
		if(sizey==8)x+=6;
		else x+=sizey/2;
	}
}

//显示汉字
void OLED_ShowChinese(u8 x,u8 y,u8 no,u8 sizey)  //(坐标+汉字下标+字号)
{
	u16 i,size1=(sizey/8+((sizey%8)?1:0))*sizey;
	for(i=0;i<size1;i++)
	{
		if(i%sizey==0) OLED_Set_Pos(x,y++);
		if(sizey==16) OLED_WR_Byte(Hzk[no][i],OLED_DATA);//16x16字号
		else return;
	}				
}