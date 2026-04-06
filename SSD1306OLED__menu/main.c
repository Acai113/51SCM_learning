#include"menu.h"
#include"timer0.h"
void main(void)
{
	Timer0_Init();
	Key_Init();
	
  Menu_Init();
  while(1) 
	{
    Menu_Loop();
  }
}