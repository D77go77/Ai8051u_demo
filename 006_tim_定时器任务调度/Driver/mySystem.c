#include "mySystem.h"

unsigned long int uwTick = 0;

void Timer2_Isr(void) interrupt 12
{
	uwTick++;
}

void Systick_Init(void)		//1毫秒@30.000MHz//time2
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0xD0;				//设置定时初始值
	T2H = 0x8A;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
}


void led_scan(void)
{
	P0 = ~P0;
}



