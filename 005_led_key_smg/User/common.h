#ifndef _ALL_H
#define _ALL_H

#pragma maxargs (60)//����Ĭ���ֽ�

#include <AI8051U.H>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "intrins.h"
#include "stc32_stc8_usb.h"

#define MAIN_Fosc        30000000UL//30Mhz
// ��������ģ����ܵ�ͷ�ļ�
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

// �����������ĵ�ַ
#define CTRL 0xa0
// ����λѡ��ʾ�ĵ�ַ
#define WEI 0xc0
// �������ʾ�ĵ�ַ
#define DUAN 0xe0
// ����ر��ſصĲ���
#define CLOSE() P2&=0x1f
// �����ָ���ſصĲ���
#define OPEN(a) P2|=a
// ����̵��������ź�
#define JDQ 1<<4
// �����������ź�
#define MOTO 1<<5
// ��������������ź�
#define FMQ 1<<6
// ����LED��ʾ�ĵ�ַ
#define LED 1<<7
// ����һ���ṹ�����ڴ洢��������
typedef struct _cj//ע��ṹ���ڴ�������⣬����������ͷ�����
{
	u16 freq; //Ƶ��
	float t; // �¶�
    u8 state; // ��״̬
    u8 h; // ʱ
    u8 m; // ��
    u8 s; // ��
    u8 d; // ����
    u8 ad; // �·�
    u8 key; // ����ֵ
    u8 adc; // ģ������ת��ֵ
};
extern struct _cj cj;

#endif 


