#include"key.h"
#include"oled.h"
#include"menu.h"

const MenuItem Menu[] = {
    // 主菜单
    {{19,20,13,15,255}, Menu_one, NULL, Menu_main,  Menu_none},  //选项一
    {{0,1,3,255}, Menu_two, NULL, Menu_main,  Menu_none},
    {{0,1,4,255}, Menu_three, NULL, Menu_main,  Menu_none},
		{{0,1,5,255}, Menu_four, NULL, Menu_main,  Menu_none},
		{{0,1,6,255}, Menu_five, NULL, Menu_main,  Menu_none},
		{{0,1,7,255}, Menu_six, NULL, Menu_main,  Menu_none},  //选项六
		
		//选项一子菜单
		{{12,13,255}, Menu_none, Change_int, Menu_one, Menu_main},
    {{18,13,255}, Menu_none, Change_float, Menu_one, Menu_main},
    {{16,17,15,255}, Menu_none, Change_step, Menu_one, Menu_main},
		
		//选项二子菜单
		{{0,1,2,255}, Menu_two_child1, NULL, Menu_two, Menu_main},
    {{0,1,3,255}, Menu_none, NULL, Menu_two, Menu_main},
    {{0,1,4,255}, Menu_none, NULL, Menu_two, Menu_main},
		
		//选项二2级菜单
		{{0,1,5,255}, Menu_none, NULL, Menu_two_child1, Menu_two},
    {{0,1,6,255}, Menu_none, NULL, Menu_two_child1, Menu_two},
    {{0,1,7,255}, Menu_none, NULL, Menu_two_child1, Menu_two},
};

#define Menu_size  (sizeof(Menu) / sizeof(Menu[0]))  //菜单项总数


unsigned char curMenu = Menu_main;
unsigned char selItem = 0;
unsigned char scroll = 0;
unsigned char fun_flag = 0;
unsigned char step = 1;

//获取这个菜单下的项数
static unsigned char GetItemCount(unsigned char menuID)
{
	unsigned char i, cnt = 0;
	for(i = 0; i < Menu_size; i++)
	{
		if(Menu[i].parent == menuID) cnt++;
	}
	return cnt;
}

//获取这个菜单第index项在menu数组中的下标
static unsigned char GetMenuIndex(unsigned char menuID, unsigned char index)
{
	unsigned char i, cnt = 0;
	for(i = 0; i < Menu_size; i++)
	{
		if(Menu[i].parent == menuID)
		{
			if(cnt == index) return i;
			cnt++;
		}
	}
	return 0;  //一般不会发生
}

//显示菜单
static void Show_menu(void)
{
	unsigned char i, j, idx, cnt, start, end;
	unsigned char line;
	
	cnt = GetItemCount(curMenu);  //获取当前菜单的项数
	start = scroll;
	end = start + 3;
	if(end > cnt) end = cnt;
	
	//OLED_Clear();
	line = 0;
	for(i = start; i < end; i++)  
	{
		idx = GetMenuIndex(curMenu, i);
		
		if(i == selItem)
		{
			OLED_ShowChar(0,line,'*',16);
			for(j=0; Menu[idx].text[j]!=255; j++)
			{
				OLED_ShowChinese((j+1)*17, line, Menu[idx].text[j], 16);
			}
		}
		else
		{
			OLED_ShowChar(0,line,' ',16);
			for(j=0; Menu[idx].text[j]!=255; j++)
			{
				OLED_ShowChinese((j+1)*17, line, Menu[idx].text[j], 16);
			}
		}
		line+=2;
	}
	
}

//返回所在菜单
static void Menu_FunBack(void) 
{
  selItem = 0;
  scroll = 0;
	OLED_Clear();
  Show_menu();
}



//进入菜单子项或执行功能
static void Menu_Enter(void)
{
	unsigned char idx, childID;
	idx = GetMenuIndex(curMenu, selItem);
	childID = Menu[idx].child;
	if(!fun_flag && childID != Menu_none)
	{
		curMenu = childID;
		selItem = 0;
		scroll = 0;
		OLED_Clear();
		Show_menu();
	}
	else if(!fun_flag)
	{
		if(Menu[idx].func != NULL)
		{
			fun_flag=1;
			Menu[idx].func();
			fun_flag=0;
			Menu_FunBack();
		}
	}
}

//返回上级菜单
static void Menu_Back(void) {
    unsigned char belongID;
    
    belongID = Menu[GetMenuIndex(curMenu, 0)].belong;
    if(belongID != curMenu && belongID != Menu_none) {
        curMenu = belongID;
        selItem = 0;
        scroll = 0;
				OLED_Clear();
        Show_menu();
    }
}



// 按键处理（基于事件）
static void Menu_KeyHandler(KeyID id, KeyEvent ev) {
    unsigned char cnt = GetItemCount(curMenu);
    
    // 处理上/下键（支持长按连续）
    if(id == KeyID_up || id == KeyID_down) {
        // 仅处理单击和重复事件（长按首次事件也可视为一次移动，与重复相同处理）
        if(ev == Event_click || ev == Event_long || ev == Event_repeat) {
            if(id == KeyID_up) {
                // 上移
                if(selItem > 0) {
                    selItem--;
                    if(selItem < scroll) 
										{
											scroll = selItem-2;
											OLED_Clear();
										}
                    Show_menu();
                }
            } else { // KeyID_down
                // 下移
                if(selItem < cnt-1) {
                    selItem++;
                    if(selItem >= scroll + 3)
										{
											scroll = selItem;
											OLED_Clear();
										}											
                    Show_menu();
                }
            }
        }
    }
    // 处理确认键（右键）
    else if(id == KeyID_left) {
        if(ev == Event_click) {
            Menu_Enter();
        }
    }
    // 处理返回键（左键）
    else if(id == KeyID_right) {
        if(ev == Event_click) {			
          if(!fun_flag) Menu_Back();
        }
    }
}

// 菜单主循环（非阻塞）
void Menu_Loop(void) {
    //static unsigned char last_key = Key_count; // 可选，用于防连发冗余
    KeyID id;
    KeyEvent ev;
    
    // 检查所有按键的事件
    for(id = 0; id < Key_count; id++) {
        ev = Key_GetEvent(id);
        if(ev != Event_none) {
            Menu_KeyHandler(id, ev);
        }
    }
}


//初始化
void Menu_Init(void) {
    curMenu = Menu_main;
    selItem = 0;
    scroll = 0;
    OLED_Init();        // 初始化OLED（假设已实现）
		OLED_Clear();
    Show_menu();
}

//以下为功能函数——————————————————————————————————————————————————————————————————————————————————————————————
//改变整形int数值
void Change_int(void)
{
	unsigned int num=0;
	char sign_flag=0;
	char need_refresh = 0; 
	KeyEvent e;
	
	OLED_Clear();
	
	OLED_ShowChinese(0,0,12,16);
	OLED_ShowChinese(17,0,13,16);
	OLED_ShowChinese(34,0,14,16);
	OLED_ShowChinese(51,0,15,16);
	OLED_ShowChar(68,0,':',16);
	OLED_ShowNum(0,3,num,16);
	while(1)
	{
		e = Key_GetEvent(KeyID_up);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			
			if(num<step)
			{ 
				num=step-num;
				sign_flag=1;
				need_refresh = 1;
			}
			else if(sign_flag)
			{
				num+=step;
				need_refresh = 1;
			}
			else 
			{	
				num-=step;
				need_refresh = 1;
			}
		}
		
		e = Key_GetEvent(KeyID_down);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			
			if(sign_flag && num<step)
			{ 
				num=step-num;
				sign_flag=0;
				need_refresh = 1;
			}
			else if(sign_flag && num>step)
			{
				num-=step;
				need_refresh = 1;
			}
			else 
			{	
				num+=step;
				need_refresh = 1;
			}
		}
		
		if(need_refresh)
		{
			if(sign_flag)
			{
				OLED_ShowChar(0,3,'-',16);
				OLED_ShowNum(8,3,num,16);
			}
			else OLED_ShowNum(0,3,num,16);
		}
		
		e = Key_GetEvent(KeyID_right);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			return;
		}
	}
}

//改变步进值
static void Change_step(void)
{
	KeyEvent e;
	char need_refresh = 0; 
	OLED_Clear();
	
	OLED_ShowChinese(0,0,16,16);
	OLED_ShowChinese(17,0,17,16);
	OLED_ShowChinese(34,0,15,16);
	OLED_ShowChar(68,0,':',16);
	OLED_ShowNum(0,3,step,16);
	
	while(1)
	{
		e = Key_GetEvent(KeyID_up);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			if(step>0) 
			{
			  step--;
				need_refresh=1;
			}
		}
		
		e = Key_GetEvent(KeyID_down);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			step++;
			need_refresh=1;
		}
		
		if(need_refresh)
		{
		  OLED_ShowNum(0,3,step,16);
			need_refresh=0;
		}
		
		e = Key_GetEvent(KeyID_right);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			return;
		}
	}
}

//显示浮点数
void Change_float(void)
{
	char cur=0, need_refresh=0, cur_refresh=0, sign_flag=0;
	unsigned int inte=0, deci=0, i;
	unsigned char len=1, l=0;
	KeyEvent e;
	OLED_Clear();
	OLED_ShowChinese(0,0,18,16);
	OLED_ShowChinese(17,0,13,16);
	OLED_ShowChinese(34,0,14,16);
	OLED_ShowChinese(51,0,15,16);
	OLED_ShowChar(68,0,':',16);
	
	OLED_ShowNum(0,3,inte,16);
	OLED_ShowChar(8,3,'.',16);
	OLED_ShowNum(16,3,deci,16);
	
	OLED_ShowChar(0,5,'^',16);
	while(1)
	{
		if(!cur)
		{
			e = Key_GetEvent(KeyID_up);
			if(e == Event_click || e == Event_repeat || e == Event_long)
			{
				if(inte<step)
				{ 
					inte=step-inte;
					sign_flag=1;
					need_refresh = 1;
				}
				else if(sign_flag)
				{
					inte+=step;
					need_refresh = 1;
				}
				else 
				{	
					inte-=step;
					need_refresh = 1;
				}
			}
			
			e = Key_GetEvent(KeyID_down);
			if(e == Event_click || e == Event_repeat || e == Event_long)
			{
				
				if(sign_flag && inte<=step)
				{ 
					inte=step-inte;
					sign_flag=0;
					need_refresh = 1;
				}
				else if(sign_flag && inte>step)
				{
					inte-=step;
					need_refresh = 1;
				}
				else 
				{	
					inte+=step;
					need_refresh = 1;
				}
			}
		}
		
		else
		{
			e = Key_GetEvent(KeyID_up);
			if(e == Event_click || e == Event_repeat || e == Event_long)
			{
				if(deci>step) 
				{
					deci-=step;
					need_refresh=1;
				}
				else deci=0;
			}
			
			e = Key_GetEvent(KeyID_down);
			if(e == Event_click || e == Event_repeat || e == Event_long)
			{
				deci+=step;
				need_refresh=1;
			}
		}
		
		e = Key_GetEvent(KeyID_left);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			cur = !cur;
			cur_refresh = 1;
		}
		
		l=0;
		i=inte;
		if(i==0) l=1;
		else
		{
			while(i>0)
			{
				i = i/10;
				l++;
			}
		}
		if(sign_flag) l++;
		if(l!=len) 
		{
			OLED_ShowString(0,3,"                ",16);
			len = l;
			cur_refresh=1;
		}
		
		if(cur_refresh)
		{
			OLED_ShowString(0,5,"                 ",16);
			if(cur==0)	OLED_ShowChar(0,5,'^',16);
			else OLED_ShowChar((len+1)*8,5,'^',16);
			cur_refresh=0;
		}
		
		if(need_refresh)
		{
			
			if(sign_flag)
			{
				OLED_ShowChar(0,3,'-',16);
				OLED_ShowNum(8,3,inte,16);
			}
			else OLED_ShowNum(0,3,inte,16);
			
			OLED_ShowChar(len*8,3,'.',16);
			OLED_ShowNum((len+1)*8,3,deci,16);
			need_refresh=0;
		}
		
		
		e = Key_GetEvent(KeyID_right);
		if(e == Event_click || e == Event_repeat || e == Event_long)
		{
			return;
		}
	}
}