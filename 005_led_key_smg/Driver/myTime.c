#include "myTime.h"
unsigned long int uwTick = 0;   //系统计时变量

void ne555_proc();

void Timer0_Init(void)		//2毫秒@30.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x05; 			// 设置为模式5（定时器模式）//计数器模式
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}


void Timer1_Isr(void) interrupt 3
{
	uwTick++;//系统总运行时长
	disp_scan(); // 调用数码管扫描函数
	ne555_proc();  // 调用时间处理函数
}

void Timer1_Init(void)		//2毫秒@30.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0xA0;				//设置定时初始值
	TH1 = 0x15;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}

// ne555处理函数
void ne555_proc()
{
    static u16 cnt_1s = 0;
    // 每500次中断（即每1s）更新频率计数并清空定时器0
    if (++cnt_1s >= 500) 
	{
        cnt_1s = 0;
		TR0=0;
		cj.freq = (TH0 << 8) | TL0; // 计算频率
		TR0=1;
		TH0 = TL0 = 0;           // 清空定时器0
    }
}