#include "ui.h"
#include "stdarg.h"

/**
 * @brief  格式化字符串并显示在指定的LCD行上。
 *
 * 该函数接受一个行号和一个格式化字符串（类似于printf），
 * 格式化字符串后，将其显示在LCD的指定行上。
 *
 * @param  Line    要显示字符串的LCD行号。
 * @param  format  格式化字符串，后跟要格式化的参数。
 *
 * 该函数内部使用 `vsprintf` 来格式化字符串，然后
 * 调用 `LCD_DisplayStringLine` 在LCD上显示格式化后的字符串。
 *
 * 示例用法:
 * @code
 * LcdSprintf(0, "Temperature: %d C", temperature);
 * @endcode
 */

void LcdSprintf(uint16_t Line, uint16_t fColour,uint16_t BColour,char *format,...)
{
    char String[21];  // 缓冲区用于存储格式化后的字符串
    va_list arg;      // 参数列表用于存储可变参数
    va_start(arg, format);  // 使用格式化字符串初始化参数列表
    vsprintf(String, format, arg);  // 格式化字符串并存储在缓冲区中
    va_end(arg);  // 清理参数列表
    //列序列;行系列(16是字号大小);字符串;字体颜色;背景颜色;字号;模式
    LCD_ShowString(0,Line*16,String,fColour,BColour,16,0);// 在LCD的指定行显示格式化后的字符串
}
/*********************************************************************
 * @fn      task_lcd_proc
 *
 * @brief   LCD任务函数
 *
 * @return  none
 */
u8 page_number = 1;
//LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
void task_lcd_proc(void)
{
    switch(page_number)
    {
        case 1:
        {
            LcdSprintf(0,BLUE,BLACK,"AI8051U");
			LcdSprintf(1,BLUE,BLACK,"CH32V307VCT6");
			LcdSprintf(2,GREEN,BLACK,"STM32F103");
			LcdSprintf(3,GREEN,BLACK,"STC89C52RC");
			LcdSprintf(4,YELLOW,BLACK,"STC15F2K60S2");
			LcdSprintf(5,YELLOW,BLACK,"RK3566");

        }break;
        case 2:
        {
            LcdSprintf(0,BLUE,BLACK,"RTC-time");
			LcdSprintf(1,BLUE,BLACK,"STC15F2K60S2");
			LcdSprintf(2,BLUE,BLACK,"STC89C52RC");
			LcdSprintf(3,BLUE,BLACK,"STM32F103");
			LcdSprintf(4,BLUE,BLACK,"CH32V307VCT6");
			LcdSprintf(5,BLUE,BLACK,"AI8051U");

        }break;
    }
}



