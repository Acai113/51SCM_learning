#include"key.h"

void Timer0_Init(void)		//@11.0592MHz
{
	TMOD &= 0xF0;			
	TMOD |= 0x01;			
	TL0 = 0x66;			
	TH0 = 0xFC;				
	TF0 = 0;				
	TR0 = 1;				
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

void Timer0_IRQ(void) interrupt 1 {
    TH0 = 0xD8;
    TL0 = 0xF0;
    
    Key_Scan();    // 每10ms扫描一次按键
}