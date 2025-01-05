#include "myTime.h"
unsigned long int uwTick = 0;   //ϵͳ��ʱ����

void ne555_proc();

void Timer0_Init(void)		//2����@30.000MHz
{
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x05; 			// ����Ϊģʽ5����ʱ��ģʽ��//������ģʽ
	TL0 = 0;				//���ö�ʱ��ʼֵ
	TH0 = 0;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}


void Timer1_Isr(void) interrupt 3
{
	uwTick++;//ϵͳ������ʱ��
	disp_scan(); // ���������ɨ�躯��
	ne555_proc();  // ����ʱ�䴦����
}

void Timer1_Init(void)		//2����@30.000MHz
{
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0xA0;				//���ö�ʱ��ʼֵ
	TH1 = 0x15;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 = 1;				//ʹ�ܶ�ʱ��1�ж�
}

// ne555������
void ne555_proc()
{
    static u16 cnt_1s = 0;
    // ÿ500���жϣ���ÿ1s������Ƶ�ʼ�������ն�ʱ��0
    if (++cnt_1s >= 500) 
	{
        cnt_1s = 0;
		TR0=0;
		cj.freq = (TH0 << 8) | TL0; // ����Ƶ��
		TR0=1;
		TH0 = TL0 = 0;           // ��ն�ʱ��0
    }
}