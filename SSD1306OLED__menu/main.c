#include"oled.h"
#include<REGX52.H>

void OLED_Set_Simple(unsigned char x, unsigned char y)
{
	unsigned char page;
	unsigned char bit_mask;
	
	page = y/8;
	bit_mask = 1<<(y%8);
	
	OLED_Write_Cmd(0xB0 + page); //
	OLED_Write_Cmd(x & 0x0F); //
	OLED_Write_Cmd(0x10 | (x >> 4)); //
	OLED_Write_Data(bit_mask); //
}

void main(void)
{
	OLED_Init();
	OLED_Clear();
	
	OLED_Set_Simple(64,32);
	while(1)
	{
		
	}
}