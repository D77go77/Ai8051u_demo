#ifndef _ALL_H
#define _ALL_H

#pragma maxargs (60)//限制默认字节

#include <AI8051U.H>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "intrins.h"
#include "stc32_stc8_usb.h"

#define MAIN_Fosc        30000000UL//30Mhz
// 包含其他模块或功能的头文件
#include "scheduler.h"
#include "myMath.h"
#include "myDelay.h"
#include "myTime.h"
#include "myKey.h"
#include "myLed.h"
#include "myCtrl.h"
#include "mySmg.h"
#include "myOnewire.h"
#include "myRtc.h"
#include "myI2c.h"
#include "myWave.h"
#include "myUart.h"

// 定义控制命令的地址
#define CTRL 0xa0
// 定义位选显示的地址
#define WEI 0xc0
// 定义段显示的地址
#define DUAN 0xe0
// 定义关闭门控的操作
#define CLOSE() P2&=0x1f
// 定义打开指定门控的操作
#define OPEN(a) P2|=a
// 定义继电器控制信号
#define JDQ 1<<4
// 定义电机控制信号
#define MOTO 1<<5
// 定义蜂鸣器控制信号
#define FMQ 1<<6
// 定义LED显示的地址
#define LED 1<<7
// 定义一个结构体用于存储测量数据
typedef struct _cj//注意结构体内存对齐问题，大的数据类型放上面
{
	u16 freq; //频率
	float t; // 温度
    u8 state; // 总状态
    u8 h; // 时
    u8 m; // 分
    u8 s; // 秒
    u8 d; // 距离
    u8 ad; // 月份
    u8 key; // 按键值
    u8 adc; // 模拟数字转换值
};
extern struct _cj cj;

#endif 


