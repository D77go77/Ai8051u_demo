#include "common.h"
#include "usb.h"


//USB调试及复位所需定义
char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";  //设置自动复位到ISP区的用户接口命令

void init(void)
{
    WTST = 0;   //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1;  //扩展寄存器(XFR)访问使能
    CKCON = 0;  //提高访问XRAM速度

    RSTFLAG |= 0x04;   //软复位标志写1清零，设置硬件复位后需要检测P3.2的状态选择运行区域，硬件复位后进入系统ISP检测到P3.2为低电平，才会进入USB下载模式

    P0M1 = 0x00;   P0M0 = 0xff;   //设置为推挽输出
    P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
    P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
    P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口

    usb_init();
    EUSB = 1;       //IE2相关的中断位操作使能后，需要重新设置EUSB
    EA = 1;
    P40 = 0;        //LED Power On
}

void main(void)
{
	init();
	while(1)
	{
		led_loop();
	}
}