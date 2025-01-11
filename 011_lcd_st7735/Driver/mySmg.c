#include "mySmg.h"

#define YANG 0
#define YIN  1

#define DISPLAY_TYPE YIN // ������ѡ���� (YANG) ���� (YIN)
/*************  ���س�������    **************/
u8 code smg[] =
{
    #if DISPLAY_TYPE == YANG
//����
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xbf,0xff,
    #elif DISPLAY_TYPE == YIN
//����
//   0    1    2    3    4    5    6    7    8    9    A    b    C    d    E    F 	-	  ��  
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40,0x00,
	#endif
};

u8 dis_buf[8]={0,0,0,0,0,0,0,0};
u8 dis_str[11];

/*************  IO�ڶ���    **************/
sbit    P_HC595_SER   = P3^4;   //pin 14    SER     data input
sbit    P_HC595_RCLK  = P3^5;   //pin 12    RCLk    store (latch) clock
sbit    P_HC595_SRCLK = P3^2;   //pin 11    SRCLK   Shift data clock

/**************** ��HC595����һ���ֽں��� ******************/
void Send_595(u8 dat)
{
    u8  i;
    for(i=0; i<8; i++)
    {
        dat <<= 1;
        P_HC595_SER   = CY;
        P_HC595_SRCLK = 1;
        P_HC595_SRCLK = 0;
    }
}
/********************** ��ʾɨ�躯�� ************************/
void task_smg_scan(void)
{
	static u8 i;  //��ʾλ����
    Send_595(dis_buf[i]);   		//�������
    Send_595(~(1<<i));            //���λ��
    P_HC595_RCLK = 1;
    P_HC595_RCLK = 0;
    if(++i >= 8)i = 0;  //8λ������0
}
/********************** �����ȡģ���� ************************/
void disp_proc()
{
    u8 i,j,tmp=0;                                 // ��ʼ��������������ʱ����
    for(i=0,j=0;i<8;i++,j++)                          // ѭ������ÿһλ�����
    {
        switch(dis_str[j])                          // �����ַ����ö�Ӧ���������ʾֵ
        {
            case '0':tmp=smg[0];break;              // �ַ� '0' ��Ӧ���������ʾֵ
            case '1':tmp=smg[1];break;              // �ַ� '1' ��Ӧ���������ʾֵ
            case '2':tmp=smg[2];break;              // �ַ� '2' ��Ӧ���������ʾֵ
            case '3':tmp=smg[3];break;              // �ַ� '3' ��Ӧ���������ʾֵ
            case '4':tmp=smg[4];break;              // �ַ� '4' ��Ӧ���������ʾֵ
            case '5':tmp=smg[5];break;              // �ַ� '5' ��Ӧ���������ʾֵ
            case '6':tmp=smg[6];break;              // �ַ� '6' ��Ӧ���������ʾֵ
            case '7':tmp=smg[7];break;              // �ַ� '7' ��Ӧ���������ʾֵ
            case '8':tmp=smg[8];break;              // �ַ� '8' ��Ӧ���������ʾֵ
            case '9':tmp=smg[9];break;              // �ַ� '9' ��Ӧ���������ʾֵ
            case 'A':tmp=smg[10];break;             // �ַ� 'A' ��Ӧ���������ʾֵ
            case 'b':tmp=smg[11];break;             // �ַ� 'b' ��Ӧ���������ʾֵ
            case 'C':tmp=smg[12];break;             // �ַ� 'C' ��Ӧ���������ʾֵ
            case 'd':tmp=smg[13];break;             // �ַ� 'd' ��Ӧ���������ʾֵ
            case 'E':tmp=smg[14];break;             // �ַ� 'E' ��Ӧ���������ʾֵ
            case 'F':tmp=smg[15];break;             // �ַ� 'F' ��Ӧ���������ʾֵ
            case '-':tmp=smg[16];break;                // �ַ� '-' ��Ӧ���������ʾֵ
            default: tmp=smg[17];break;                 // Ĭ������µ��������ʾֵ
        }
        if(dis_str[j+1]=='.')                       // �����һ���ַ��Ƿ�ΪС����
        {
			#if DISPLAY_TYPE == YANG
				tmp&=0x7f;//���������
			#elif DISPLAY_TYPE == YIN
				tmp|=0x80;//���������
			#endif
            j++;                                    // ����С�����ַ�
        }
        dis_buf[i]=tmp;                             // ���������ֵ������ʾ������
    }
}
//**************************************************************//
void task_smg_proc(void)
{
    switch(cj.state)                                // ����״̬ѡ��ͬ����ʾ��ʽ
    {
        case 10:
                sprintf(dis_str,"10A--FEE"); 
		break;
        case 20:
                sprintf(dis_str,"20A--FEE"); 
		break;
        case 30:
                sprintf(dis_str,"30A--FEE"); 
		break;
        case 40:
                sprintf(dis_str,"40A--FEE");
		break;
    }
    disp_proc();                                    // ���� disp_proc ����������ʾ����
}