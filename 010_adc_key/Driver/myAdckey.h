#ifndef MY_ADC_KEY_H
#define MY_ADC_KEY_H

#include  "common.h"

// 定义一个结构体，用于存储 ADC 按键的相关变量
typedef struct
{
    uint8_t KeyState;       // 当前键状态
    uint8_t KeyState1;      // 最近一次的键状态
    uint8_t KeyState2;      // 前一次的键状态
    uint8_t KeyState3;      // 更前一次的键状态
    uint8_t KeyHoldCnt;     // 按键按下计时
    uint8_t KeyCode;        // 当前检测到的键码 (1~16 有效)
} AdcKeyHandle;

// 定义 ADC 键盘相关函数的接口

void nADC_init(void);
void task_adc_key_proc(void);


#endif
