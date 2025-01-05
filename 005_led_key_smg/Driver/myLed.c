#include "myLed.h"

volatile  u8  bdata led_dat=0;
sbit L1 = led_dat^0;
sbit L2 = led_dat^1;
sbit L3 = led_dat^2;
sbit L4 = led_dat^3;
sbit L5 = led_dat^4;
sbit L6 = led_dat^5;
sbit L7 = led_dat^6;
sbit L8 = led_dat^7;

u8 f_on=0;//led��˸��־
/**
 * led_scan��������ɨ��LED״̬������Ӳ�����ơ�
 * �ú�����led_dat����ȡ����ֵ���˿�P0��������LED��ʾ��
 * ������gate��������LED�ĵ���״̬��
 */
void led_scan()
{
    P0=~led_dat;
    gate(LED);
}

/**
 * led_proc�������ڴ���LED����ʾ�߼���
 * �ú�����������led_dat������Ȼ�����cj.state��״̬����LED����ʾģʽ��
 */
void led_proc()
{
    led_dat=0;
    switch(cj.state)
    {
        case 10:
            L1=L3=1;
        break;
		
		case 20:
			L2=L4=1;
		break;

		case 30:
			L3=L6=1;
		break;
		
		case 40:
			L4=L7=1;
		break;
    }
}
//***************
void led_smg_blink()
{
	// ÿ500ms�л�f_wei��f_on��״̬ʵ��1s���ڱ仯
	f_wei = !f_wei; // �л�f_wei״̬
	f_on = !f_on;   // �л�f_on״̬
}