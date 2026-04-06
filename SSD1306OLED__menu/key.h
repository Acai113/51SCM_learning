#ifndef __KEY_H__
#define __KEY_H__

#include<REGX52.H>

sbit Key_up = P3^1;  //上/加
sbit Key_down = P3^0;  //下/减
sbit Key_left = P3^2;  //左/取消
sbit Key_right = P3^3;  //右/确定



//按键ID
typedef enum{
	KeyID_up,
	KeyID_down,
	KeyID_left,
	KeyID_right,
	Key_count
}KeyID;

//按键事件
typedef enum{
	Event_none = 0, //无事件
	Event_click,  //单击
	Event_long,  //单次连按
	Event_repeat  //持续连按
}KeyEvent;

static void Key_Update(KeyID id);  //按键状态更新
void Key_Init(void);  //按键初始化
void Key_Scan(void);  //按键扫描
KeyEvent Key_GetEvent(KeyID id);  //获取事件	

#endif