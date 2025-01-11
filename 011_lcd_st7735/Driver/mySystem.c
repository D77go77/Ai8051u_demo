#include "mySystem.h"

unsigned long int uwTick = 0;

void Timer2_Isr(void) interrupt 12
{
	uwTick++;
}

void Systick_Init(void)		//1����@30.000MHz//time2
{
	AUXR |= 0x04;			//��ʱ��ʱ��1Tģʽ
	T2L = 0xD0;				//���ö�ʱ��ʼֵ
	T2H = 0x8A;				//���ö�ʱ��ʼֵ
	AUXR |= 0x10;			//��ʱ��2��ʼ��ʱ
	IE2 |= 0x04;			//ʹ�ܶ�ʱ��2�ж�
}

/*********************************************************************
 * @fn      micros
 *
 * @brief   micros.
 *
 * @return  ���� us
 */
uint32_t micros(void)
{
  return uwTick;
}
/*********************************************************************
 * @fn      millis
 *
 * @brief   millis.
 *
 * @return  ���� ms
 */
uint32_t millis(void) 
{
    return micros() / 1000UL;
}

