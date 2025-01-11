#include "common.h"
struct _cj cj;

//USB���Լ���λ���趨��
char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";  //�����Զ���λ��ISP�����û��ӿ�����

void sysytem_init(void)
{
	WTST = 0;   //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXFR = 1;  //��չ�Ĵ���(XFR)����ʹ��
	CKCON = 0;  //��߷���XRAM�ٶ�

	P_SW2 |= 0x80;		//B7λд1��ʹ�ܷ���XFR
	//������IO�ھ���Ϊ׼˫���ģʽ����������ô���
	P0M1 = 0x00;   P0M0 = 0x00;
	P1M1 = 0x00;   P1M0 = 0x00;
	P2M1 = 0x00;   P2M0 = 0x00;
	P3M1 = 0x00;   P3M0 = 0x00;
	P4M1 = 0x00;   P4M0 = 0x00;
	P5M1 = 0x00;   P5M0 = 0x00;
	P6M1 = 0x00;   P6M0 = 0x00;
	P7M1 = 0x00;   P7M0 = 0x00;
	usb_init();                                     //USB CDC �ӿ�����
    IE2 |= 0x80;                                    //ʹ��USB�ж�
}

void struct_init(void)
{
	cj.state = 10;
}

void driver_init(void)
{
	nGpio_init();
//	nKey_init();
	nADC_init();
    // ��ʼ��������
    Scheduler_Init();
	//��ʱ��2��ϵͳ�δ�ʱ��
	Systick_Init();
}

void init(void)
{	
	sysytem_init();
	struct_init();
	driver_init();
	EA = 1;											//IE |= 0X80;�������ж�
	while (DeviceState != DEVSTATE_CONFIGURED);     //�ȴ�USB������� //����ע��
    P40 = 0;        //LED Power On
}

void main(void)
{
	init();
	while(1)
	{
		Scheduler_Run();// ���е�����
	}
}

