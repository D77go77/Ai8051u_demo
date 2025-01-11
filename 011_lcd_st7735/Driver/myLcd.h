#ifndef MY_LCD_H
#define MY_LCD_H

#include "common.h"
//-----------------LCD端口定义----------------
/*	定义接口	*/
							//GND	AI8051U实验箱 V1.1
							//VCC	3~5V
sbit P_LCD_CLK	=  	P3^2;	//D0	SPI or II2 的时钟脚
sbit P_LCD_SDA	=   P3^3;	//D1	SPI or II2 的数据脚
sbit P_LCD_RST	=  	P4^7;	//RES	复位脚, 低电平复位
sbit P_LCD_DC	=  	P1^1;	//DC	数据或命令脚
sbit P_LCD_CS	=	P3^5;	//CS	片选脚

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
// 显示定义
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160
#else
#define LCD_W 160
#define LCD_H 128
#endif

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


void delay_ms(unsigned int ms);//不准确延时函数
void LCD_GPIO_Init(void);//初始化GPIO
void LCD_SendByte(u8 dat);
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA16(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

extern	u16	SPI_TxCnt;		//SPI DMA触发发送次数, 一次128字节, 一共8次
extern	bit	B_SPI_DMA_busy;	//SPI DMA忙标志， 1标志SPI-DMA忙，SPI DMA中断中清除此标志，使用SPI DMA前要确认此标志为0
extern	u16	SPI_TxAddr;		//SPI DMA要发送数据的首地址
extern	bit	B_TxCmd;		//已发送命令标志
extern	u8 xdata CmdTmp[];	//命令缓冲
extern	u8	xdata DisTmp[3200];	//显示缓冲，将要显示的内容放在显存里，启动DMA即可. 由于LCM DMA有4字节对齐问题，所以这里定位对地址为4的倍数
void	SPI_DMA_TRIG(u8 xdata *TxBuf);

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//指定区域填充颜色
void LCD_DrawPoint(u16 x,u16 y,u16 color);//在指定位置画一个点
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//在指定位置画一条线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//在指定位置画一个矩形
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//在指定位置画一个圆

void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示汉字串
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个32x32汉字

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串
u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width, u8 xdata pic[]);//显示图片


void nLcd_init(void);





#endif

