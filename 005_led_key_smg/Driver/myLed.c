#include "myLed.h"

volatile  u8  bdata led_dat=0;
sbit L1 = led_dat^0;
sbit L2 = led_dat^1;
sbit L3 = led_dat^2;
sbit L4 = led_dat^3;
sbit L5 = led_dat^4;
sbit L6 = led_dat^5;
sbit L7 = led_dat^6;
sbit L8 = led_dat^7;

u8 f_on=0;//led闪烁标志
/**
 * led_scan函数用于扫描LED状态并进行硬件控制。
 * 该函数将led_dat变量取反后赋值给端口P0，以驱动LED显示，
 * 并调用gate函数控制LED的点亮状态。
 */
void led_scan()
{
    P0=~led_dat;
    gate(LED);
}

/**
 * led_proc函数用于处理LED的显示逻辑。
 * 该函数首先清零led_dat变量，然后根据cj.state的状态设置LED的显示模式。
 */
void led_proc()
{
    led_dat=0;
    switch(cj.state)
    {
        case 10:
            L1=L3=1;
        break;
		
		case 20:
			L2=L4=1;
		break;

		case 30:
			L3=L6=1;
		break;
		
		case 40:
			L4=L7=1;
		break;
    }
}
//***************
void led_smg_blink()
{
	// 每500ms切换f_wei和f_on的状态实现1s周期变化
	f_wei = !f_wei; // 切换f_wei状态
	f_on = !f_on;   // 切换f_on状态
}