#include "common.h"
struct _cj cj;


//USB调试及复位所需定义
char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";  //设置自动复位到ISP区的用户接口命令

void init(void)
{
    WTST = 0;   //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1;  //扩展寄存器(XFR)访问使能
    CKCON = 0;  //提高访问XRAM速度

    P_SW2 |= 0x80;		//B7位写1，使能访问XFR
	//将所有IO口均设为准双向口模式，方便后续敲代码
    P0M1 = 0x00;   P0M0 = 0x00;
    P1M1 = 0x00;   P1M0 = 0x00;
    P2M1 = 0x00;   P2M0 = 0x00;
    P3M1 = 0x00;   P3M0 = 0x00;
    P4M1 = 0x00;   P4M0 = 0x00;
    P5M1 = 0x00;   P5M0 = 0x00;
    P6M1 = 0x00;   P6M0 = 0x00;
    P7M1 = 0x00;   P7M0 = 0x00;
	
	usb_init();                                     //USB CDC 接口配置
    IE2 |= 0x80;                                    //使能USB中断
	

	// 初始化控制结构体cj
	cj.state=10;
	cj.h=0;cj.m=1;cj.s=21;
	cj.key=0;cj.freq = 0;
	// 初始化继电器和蜂鸣器
	off(JDQ);off(FMQ);
	// 初始化实时时钟、AD模块和温度传感器
	rtc_run(0x00,0x01,0x21);
	init_ad(0x43);
	init_temp();
    // 初始化调度器
    Scheduler_Init();
	
    // 初始化定时器0和定时器1
    Timer0_Init();
    Timer1_Init();//习惯初始化中断定时器放最后//
	
	EA = 1;											//IE |= 0X80;开启总中断
	
	while (DeviceState != DEVSTATE_CONFIGURED);     //等待USB完成配置 //仿真注释
    P40 = 0;        //LED Power On
}

void main(void)
{
	init();
	while(1)
	{
		Scheduler_Run();// 运行调度器
	}
}