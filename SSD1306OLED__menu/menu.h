#ifndef __MENU_H__
#define __MENU_H__

#include"key.h"
#include"oled.h"

#define Menu_main 0
#define Menu_one 1
#define Menu_two 2
#define Menu_three 3
#define Menu_four 4
#define Menu_five 5
#define Menu_six 6
#define Menu_seven 7
#define Menu_eight 8
#define Menu_nine 9
#define Menu_two_child1 9
#define Menu_none 255
#define NULL 0

//菜单项结构体
typedef struct {
    unsigned char text[5];             // 菜单文字,255表示结束
    unsigned char child;    // 子菜单ID，MENU_NONE表示无子菜单
    void (*func)(void);     // 功能函数（叶子节点时有效）
    unsigned char parent;   // 所在菜单ID
		unsigned char belong;  //  父菜单ID
} MenuItem;

extern unsigned char curMenu;      // 当前菜单ID
extern unsigned char selItem;       // 当前选中项（0~3，屏幕上的行）
extern unsigned char scroll;     // 滚动偏移（从第几项开始显示）
extern unsigned char fun_flag;
extern unsigned char step;  //修改数字时的步进值，初始值为1 

void Menu_Init(void);  //初始化
void Menu_Loop(void);  //主循环，在main函数中调用
static unsigned char GetItemCount(unsigned char menuID);
static unsigned char GetMenuIndex(unsigned char menuID, unsigned char index);
static void Show_menu(void);

//===功能函数===
void Change_int(void);
void Change_float(void);
void Change_step(void);

#endif