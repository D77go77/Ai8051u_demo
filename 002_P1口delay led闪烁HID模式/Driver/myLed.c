#include "myLed.h"



void led_loop(void)
{
	P10 = 0;			//����
	delay_ms(200);
	P10 = 1;			//Ϩ��
	delay_ms(200);
	P10 = 0;			//����
	delay_ms(200);
	P10 = 1;			//Ϩ��
	delay_ms(200);
	P10 = 0;			//����
	delay_ms(200);
	P10 = 1;			//Ϩ��
	delay_ms(200);

	P10 = 0;			//����
	delay_ms(500);
	P10 = 1;			//Ϩ��
	delay_ms(500);
	P10 = 0;			//����
	delay_ms(500);
	P10 = 1;			//Ϩ��
	delay_ms(500);
	P10 = 0;			//����
	delay_ms(500);
	P10 = 1;			//Ϩ��
	delay_ms(500);
	
	P10 = 0;			//����
	delay_ms(200);
	P10 = 1;			//Ϩ��
	delay_ms(200);
	P10 = 0;			//����
	delay_ms(200);
	P10 = 1;			//Ϩ��
	delay_ms(200);
	P10 = 0;			//����
	delay_ms(200);
	P10 = 1;			//Ϩ��
	delay_ms(200);	
	
	delay_ms(3000);	
}


