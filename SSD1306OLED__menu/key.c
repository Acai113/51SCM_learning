#include"key.h"
#include"timer0.h"

#define SCAN_MS      10      // 扫描间隔10ms
#define DEBOUNCE_MS  10      // 消抖时间20ms
#define SHORT_MS     60    // 单击判定时间200ms
#define LONG_MS      1000    // 长按判定时间1000ms
#define REPEAT_MS    100     // 连发间隔100ms

// 转换为扫描次数
#define Debounce_cnt  (DEBOUNCE_MS / SCAN_MS)   // 2次
#define Short_cnt    (SHORT_MS / SCAN_MS)      // 20次
#define Long_cnt      (LONG_MS / SCAN_MS)       // 100次
#define Repeat_cnt    (REPEAT_MS / SCAN_MS)     // 10次

//按键状态
typedef struct{
	unsigned char state; //0空闲 1消抖 2按下 3长按 4连发
	unsigned char cnt;  //计数器
	unsigned char event;  //事件
	unsigned char last;           //上次读到的电平
}Key;

static Key keys[Key_count];

static bit Key_read(KeyID id) {
    switch(id) {
        case KeyID_up:    return Key_up;
        case KeyID_down:  return Key_down;
        case KeyID_left:  return Key_left;
        case KeyID_right: return Key_right;
        default:           return 1;
    }
}

static void Key_Update(KeyID id)
{
	Key *k = &keys[id];
	bit now = Key_read(id);
	
	switch(k->state)
	{
		case 0:        //空闲状态
			if(now == 0)
			{
				k->state = 1;  //进入消抖状态
				k->cnt = Debounce_cnt;  //消抖扫描2次 
			}
			break;
			
		case 1:  //消抖状态	
			if(now == 0)
			{
				if(k->cnt > 0) k->cnt--;  //扫描次数没到，继续消抖
				
				if(k->cnt == 0) 
				{
					k->state = 2;  //消抖结束，进入按下状态
					k->cnt = 0;
				}
			}	
			
			else k->state = 0;  //是抖动，回到空闲
			break;
			
		case 2:  //按下计时
			if(now == 0)
			{
				k->cnt++;

				if(k->cnt >= Long_cnt)  //达到长按时间
				{
					k->state = 3;  //长按状态
					k->event = Event_long;
				}
			}
			
			else  //按键释放
			{
				if(k->cnt >= Short_cnt) k->event = Event_click;  //单击
				k->state = 0;
			}
			break;
			
		case 3:  //长按确认
			if(now == 0) 
			{
				k->state = 4;  //进入连发
				k->cnt = Repeat_cnt;
			}
			
			else k->state = 0;  //释放，回到空闲
			break;
			
		case 4:  //连发状态
			if(now == 0)
			{
				if(k->cnt > 0) k->cnt--;
				if(k->cnt == 0)
				{
					k->event = Event_repeat;
					k->cnt = Repeat_cnt;
				}
			}
			
			else k->state = 0;
			break;
	}
	
	k->last = now;
}

//初始化
void Key_Init(void)
{
	unsigned char i;
	for(i = 0; i<Key_count; i++)
	{
		keys[i].state = 0;
		keys[i].cnt = 0;
		keys[i].event = Event_none;
		keys[i].last = 1;
	}
}

//扫描按键
void Key_Scan(void)
{
	unsigned char i;
	for(i=0; i<Key_count; i++)
	{
		Key_Update(i);
	}
}

//获取按键事件
KeyEvent Key_GetEvent(KeyID id)
{
	KeyEvent e;
	if(id >= Key_count) return Event_none;
	
	e = keys[id].event;
	keys[id].event = Event_none;  //重置状态
	return e;
}
