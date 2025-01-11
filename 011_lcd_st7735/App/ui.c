#include "ui.h"
#include "stdarg.h"

/**
 * @brief  ��ʽ���ַ�������ʾ��ָ����LCD���ϡ�
 *
 * �ú�������һ���кź�һ����ʽ���ַ�����������printf����
 * ��ʽ���ַ����󣬽�����ʾ��LCD��ָ�����ϡ�
 *
 * @param  Line    Ҫ��ʾ�ַ�����LCD�кš�
 * @param  format  ��ʽ���ַ��������Ҫ��ʽ���Ĳ�����
 *
 * �ú����ڲ�ʹ�� `vsprintf` ����ʽ���ַ�����Ȼ��
 * ���� `LCD_DisplayStringLine` ��LCD����ʾ��ʽ������ַ�����
 *
 * ʾ���÷�:
 * @code
 * LcdSprintf(0, "Temperature: %d C", temperature);
 * @endcode
 */

void LcdSprintf(uint16_t Line, uint16_t fColour,uint16_t BColour,char *format,...)
{
    char String[21];  // ���������ڴ洢��ʽ������ַ���
    va_list arg;      // �����б����ڴ洢�ɱ����
    va_start(arg, format);  // ʹ�ø�ʽ���ַ�����ʼ�������б�
    vsprintf(String, format, arg);  // ��ʽ���ַ������洢�ڻ�������
    va_end(arg);  // ��������б�
    //������;��ϵ��(16���ֺŴ�С);�ַ���;������ɫ;������ɫ;�ֺ�;ģʽ
    LCD_ShowString(0,Line*16,String,fColour,BColour,16,0);// ��LCD��ָ������ʾ��ʽ������ַ���
}
/*********************************************************************
 * @fn      task_lcd_proc
 *
 * @brief   LCD������
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



