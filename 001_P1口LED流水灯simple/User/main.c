#include "common.h"
#include "usb.h"


//USB���Լ���λ���趨��
char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";  //�����Զ���λ��ISP�����û��ӿ�����

void init(void)
{
    WTST = 0;   //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1;  //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0;  //��߷���XRAM�ٶ�

    RSTFLAG |= 0x04;   //��λ��־д1���㣬����Ӳ����λ����Ҫ���P3.2��״̬ѡ����������Ӳ����λ�����ϵͳISP��⵽P3.2Ϊ�͵�ƽ���Ż����USB����ģʽ

    P0M1 = 0x00;   P0M0 = 0xff;   //����Ϊ�������
    P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
    P2M1 = 0x00;   P2M0 = 0x00;   //����Ϊ׼˫���
    P3M1 = 0x00;   P3M0 = 0x00;   //����Ϊ׼˫���
    P4M1 = 0x00;   P4M0 = 0x00;   //����Ϊ׼˫���
    P5M1 = 0x00;   P5M0 = 0x00;   //����Ϊ׼˫���
    P6M1 = 0x00;   P6M0 = 0x00;   //����Ϊ׼˫���
    P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���

    usb_init();
    EUSB = 1;       //IE2��ص��ж�λ����ʹ�ܺ���Ҫ��������EUSB
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