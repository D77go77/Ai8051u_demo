#include "mySmg.h"

/*************  本地常量声明    **************/
u8 code smg[] =
{
//共阳
//0xc0, //0
//0xf9, //1
//0xa4, //2
//0xb0, //3
//0x99, //4
//0x92, //5
//0x82, //6
//0xf8, //7
//0x80, //8
//0x90, //9
//0x88, //A
//0x83, //b
//0xc6, //C
//0xa1, //d
//0x86, //E
//0x8e //F
//共阴
//   0    1    2    3    4    5    6    7    8    9    A    b    C    d    E    F -
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40,
};

u8 dis_buf[8]={0,0,0,0,0,0,0,0};
u8 dis_str[11];

/*************  IO口定义    **************/
sbit    P_HC595_SER   = P3^4;   //pin 14    SER     data input
sbit    P_HC595_RCLK  = P3^5;   //pin 12    RCLk    store (latch) clock
sbit    P_HC595_SRCLK = P3^2;   //pin 11    SRCLK   Shift data clock

/**************** 向HC595发送一个字节函数 ******************/
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
/********************** 显示扫描函数 ************************/
void task_smg_scan(void)
{
	static u8 i;  //显示位索引
    Send_595(dis_buf[i]);   		//输出段码
    Send_595(~(1<<i));            //输出位码
    P_HC595_RCLK = 1;
    P_HC595_RCLK = 0;
    if(++i >= 8)i = 0;  //8位结束回0
}
/********************** 数码管取模函数 ************************/
void disp_proc()
{
    u8 i,j,tmp=0;                                 // 初始化索引变量和临时变量
    for(i=0,j=0;i<8;i++,j++)                          // 循环处理每一位数码管
    {
        switch(dis_str[j])                          // 根据字符设置对应的数码管显示值
        {
            case '0':tmp=smg[0];break;              // 字符 '0' 对应的数码管显示值
            case '1':tmp=smg[1];break;              // 字符 '1' 对应的数码管显示值
            case '2':tmp=smg[2];break;              // 字符 '2' 对应的数码管显示值
            case '3':tmp=smg[3];break;              // 字符 '3' 对应的数码管显示值
            case '4':tmp=smg[4];break;              // 字符 '4' 对应的数码管显示值
            case '5':tmp=smg[5];break;              // 字符 '5' 对应的数码管显示值
            case '6':tmp=smg[6];break;              // 字符 '6' 对应的数码管显示值
            case '7':tmp=smg[7];break;              // 字符 '7' 对应的数码管显示值
            case '8':tmp=smg[8];break;              // 字符 '8' 对应的数码管显示值
            case '9':tmp=smg[9];break;              // 字符 '9' 对应的数码管显示值
            case 'A':tmp=smg[10];break;             // 字符 'A' 对应的数码管显示值
            case 'b':tmp=smg[11];break;             // 字符 'b' 对应的数码管显示值
            case 'C':tmp=smg[12];break;             // 字符 'C' 对应的数码管显示值
            case 'd':tmp=smg[13];break;             // 字符 'd' 对应的数码管显示值
            case 'E':tmp=smg[14];break;             // 字符 'E' 对应的数码管显示值
            case 'F':tmp=smg[15];break;             // 字符 'F' 对应的数码管显示值
            case '-':tmp=smg[16];break;                // 字符 '-' 对应的数码管显示值
            default:tmp=0x00;break;                 // 默认情况下的数码管显示值
        }
        if(dis_str[j+1]=='.')                       // 检查下一个字符是否为小数点
        {
            //tmp&=0x7f;//共阳数码管
			tmp|=0x80;//共阴数码管
            j++;                                    // 跳过小数点字符
        }
        dis_buf[i]=tmp;                             // 将处理后的值存入显示缓冲区
    }
}
//**************************************************************//
void task_smg_proc(void)
{
	int i = 10;
    switch(i)                                // 根据状态选择不同的显示格式
    {
        case 10:
                sprintf(dis_str,"E--F1.234"); // 格式化字符串，显示频率和时间
		break;
    }
    disp_proc();                                    // 调用 disp_proc 函数进行显示处理
}
