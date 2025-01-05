#include "mySmg.h"
unsigned char  smg[] =
{
0xc0, //0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0x88, //A
0x83, //b
0xc6, //C
0xa1, //d
0x86, //E
0x8e //F
};

u8 dis_buf[8]={0,0,0,0,0,0,0,0};
char dis_str[10];
//**************************************************************//
void disp_scan()
{
    static u8 i=0;                                // ʹ�þ�̬���� i ���䵱ǰɨ��λ��
    P0=0;                                         // �ر����������
    gate(WEI);                                    // ����λѡ�ź�
    P0=dis_buf[i];                                // ��ʾ��ǰλ����ܵ�����
    gate(DUAN);                                   // ���ö�ѡ�ź�
    P0=1<<i;                                      // ѡ��ǰλ�����
    gate(WEI);                                    // ����λѡ�ź�
    if(++i==8)i=0;                                // ����ɨ��λ�ã����ﵽ���ֵ������Ϊ0
}
//**************************************************************//
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
            case '-':tmp=0xbf;break;                // �ַ� '-' ��Ӧ���������ʾֵ
            default:tmp=0xff;break;                 // Ĭ������µ��������ʾֵ
        }
        if(dis_str[j+1]=='.')                       // �����һ���ַ��Ƿ�ΪС����
        {
            tmp&=0x7f;                              // �����С���㣬�����С����
            j++;                                    // ����С�����ַ�
        }
        dis_buf[i]=tmp;                             // ��������ֵ������ʾ������
    }
}
//**************************************************************//



float  temp12 = 123.123;			
u8 f_wei=0;//smg��˸��־
void smg_proc()
{


    switch(cj.state)                                // ����״̬ѡ��ͬ����ʾ��ʽ
    {
        case 10:
            if(f_wei)
                sprintf(dis_str,"F%02u-%02ubb",cj.m,cj.s); // ��ʽ���ַ�������ʾƵ�ʺ�ʱ��
            else
                sprintf(dis_str,"   -%02ubb",cj.s);         // ��ʽ���ַ�������ʾʱ��
            break;
		break;
        case 20:
            if(f_wei)
                sprintf(dis_str,"d    %3u",cj.key);          // ��ʽ���ַ�������ʾ����ֵ
            else
                sprintf(dis_str,"E  %5u",cj.freq);           // ��ʽ���ַ�������ʾƵ��
            break;
        case 30:sprintf(dis_str,"b%3.1f %3u",cj.t,cj.d);break; // ��ʽ���ַ�������ʾ�¶Ⱥ;���
        case 40:sprintf(dis_str,"A    %3u",cj.ad);break;     // ��ʽ���ַ�������ʾADֵ
    }
    disp_proc();                                    // ���� disp_proc ����������ʾ����
}







			
			
		
