#include "myLcd.h"
#include "lcdfont.h"
#include "lcdpic.h"

//-----------------变量定义----------------
bit	B_SPI_DMA_busy;		//SPI DMA忙标志， 1标志SPI-DMA忙，SPI DMA中断中清除此标志，使用SPI DMA前要确认此标志为0
u16	SPI_TxAddr;			//SPI DMA要发送数据的首地址
u8	xdata DisTmp[3200];	//显示缓冲，将要显示的内容放在显存里，启动DMA即可. 由于LCM DMA有4字节对齐问题，所以这里定位对地址为4的倍数

//N ms延时函数
void delay_ms(u16 ms)	// 1~65535 ms
{
	unsigned long edata i;
	while(ms--)
	{
	_nop_();
	_nop_();
	_nop_();
	i = 7498UL;
	while (i) i--;
	}
}
//========================================================================
// 函数: void  SPI_Config(u8 SPI_io, u8 SPI_speed)
// 描述: SPI初始化函数。
// 参数: io: 切换到的IO,            SS  MOSI MISO SCLK
//                       0: 切换到 P1.4 P1.5 P1.6 P1.7
//                       1: 切换到 P2.4 P2.5 P2.6 P2.7
//                       2: 切换到 P4.0 P4.1 P4.2 P4.3
//                       3: 切换到 P3.5 P3.4 P3.3 P3.2
//       SPI_speed: SPI的速度, 0: fosc/4,  1: fosc/8,  2: fosc/16,  3: fosc/2
// 返回: none.
//========================================================================
void  SPI_Config(u8 SPI_io, u8 SPI_speed)
{
	SPI_io &= 3;

	SPCTL = SPI_speed & 3;	//配置SPI 速度, 这条指令先执行, 顺便Bit7~Bit2清0
	SSIG = 1;	//1: 忽略SS脚，由MSTR位决定主机还是从机		0: SS脚用于决定主机还是从机。
	SPEN = 1;	//1: 允许SPI，								0：禁止SPI，所有SPI管脚均为普通IO
	DORD = 0;	//1：LSB先发，								0：MSB先发
	MSTR = 1;	//1：设为主机								0：设为从机
	CPOL = 1;	//1: 空闲时SCLK为高电平，					0：空闲时SCLK为低电平
	CPHA = 1;	//1: 数据在SCLK前沿驱动,后沿采样.			0: 数据在SCLK前沿采样,后沿驱动.
//	SPR1 = 0;	//SPR1,SPR0   00: fosc/4,     01: fosc/8
//	SPR0 = 0;	//            10: fosc/16,    11: fosc/2
	P_SW1 = (P_SW1 & ~0x0c) | ((SPI_io<<2) & 0x0c);		//切换IO

	HSCLKDIV   = 1;					//HSCLKDIV主时钟分频
	SPI_CLKDIV = 1;					//SPI_CLKDIV主时钟分频
	SPSTAT = 0x80 + 0x40;			//清0 SPIF和WCOL标志

	if(SPI_io == 0)
	{
		P1n_standard(0xf0);			//切换到 P1.4(SS) P1.5(MOSI) P1.6(MISO) P1.7(SCLK), 设置为准双向口
		PullUpEnable(P1PU, 0xf0);	//设置上拉电阻    允许端口内部上拉电阻   PxPU, 要设置的端口对应位为1
		P1n_push_pull(Pin7+Pin5);	//MOSI SCLK设置为推挽输出
		SlewRateHigh(P1SR, Pin7+Pin5);	//MOSI SCLK端口输出设置为高速模式   PxSR, 要设置的端口对应位为1.    高速模式在3.3V供电时速度可以到13.5MHz(27MHz主频，SPI速度2分频)
	}
	else if(SPI_io == 1)
	{
		P2n_standard(0xf0);			//切换到P2.4(SS) P2.5(MOSI) P2.6(MISO) P2.7(SCLK), 设置为准双向口
		PullUpEnable(P2PU, 0xf0);	//设置上拉电阻    允许端口内部上拉电阻   PxPU, 要设置的端口对应位为1
		P2n_push_pull(Pin7+Pin5);	//MOSI SCLK设置为推挽输出
		SlewRateHigh(P2SR, Pin7+Pin5);	//MOSI SCLK端口输出设置为高速模式   PxSR, 要设置的端口对应位为1.    高速模式在3.3V供电时速度可以到13.5MHz(27MHz主频，SPI速度2分频)
	}
	else if(SPI_io == 2)
	{
		P4n_standard(0x0f);			//切换到P4.0(SS) P4.1(MOSI) P4.2(MISO) P4.3(SCLK), 设置为准双向口
		PullUpEnable(P4PU, 0x0f);	//设置上拉电阻    允许端口内部上拉电阻   PxPU, 要设置的端口对应位为1
		P4n_push_pull(Pin3+Pin1);	//MOSI SCLK设置为推挽输出
		SlewRateHigh(P4SR, Pin3+Pin1);	//MOSI SCLK端口输出设置为高速模式   PxSR, 要设置的端口对应位为1.    高速模式在3.3V供电时速度可以到13.5MHz(27MHz主频，SPI速度2分频)
	}
	else if(SPI_io == 3)
	{
		P3n_standard(0x3C);		//切换到P3.5(SS) P3.4(MOSI) P3.3(MISO) P3.2(SCLK), 设置为准双向口
		PullUpEnable(P3PU, 0x3c);	//设置上拉电阻    允许端口内部上拉电阻   PxPU, 要设置的端口对应位为1
		P3n_push_pull(Pin4+Pin2);	//MOSI SCLK设置为推挽输出
		SlewRateHigh(P3SR, Pin4+Pin2);	//MOSI SCLK端口输出设置为高速模式   PxSR, 要设置的端口对应位为1.    高速模式在3.3V供电时速度可以到13.5MHz(27MHz主频，SPI速度2分频)
	}
}
//========================================================================
// 描述: 写SPI一个字节函数
// 参数: 无.
// 返回: 无
//========================================================================

void	LCD_SendByte(u8 dat)
{
	SPDAT = dat;	//发送一个字节
	while(SPIF == 0)	;			//等待发送完成
	SPSTAT = 0x80 + 0x40;			//清0 SPIF和WCOL标志
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	P_LCD_CS = 0;
	LCD_SendByte(dat);
	P_LCD_CS = 1;
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA16(u16 dat)
{
	P_LCD_CS = 0;
	LCD_SendByte((u8)(dat>>8));
	LCD_SendByte((u8)dat);
	P_LCD_CS = 1;
}
/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	P_LCD_DC = 0;//写命令
	P_LCD_CS = 0;
	LCD_SendByte(dat);
	P_LCD_CS = 1;
	P_LCD_DC = 1;//写数据
}
/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
    if(USE_HORIZONTAL==0)
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA16(x1+2);
        LCD_WR_DATA16(x2+2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA16(y1+1);
        LCD_WR_DATA16(y2+1);
        LCD_WR_REG(0x2c);//储存器写
    }
    else if(USE_HORIZONTAL==1)
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA16(x1+2);
        LCD_WR_DATA16(x2+2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA16(y1+3);
        LCD_WR_DATA16(y2+3);
        LCD_WR_REG(0x2c);//储存器写
    }
    else if(USE_HORIZONTAL==2)
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA16(x1+1);
        LCD_WR_DATA16(x2+1);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA16(y1+2);
        LCD_WR_DATA16(y2+2);
        LCD_WR_REG(0x2c);//储存器写
    }
    else
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA16(x1+3);
        LCD_WR_DATA16(x2+3);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA16(y1+2);
        LCD_WR_DATA16(y2+2);
        LCD_WR_REG(0x2c);//储存器写
    }
}


/******************************************************************************
      函数说明：LCD初始化函数
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_Init(void)
{
	P_LCD_CLK = 1;//SCLK
	P_LCD_SDA = 1;//MOSI
	P_LCD_RST = 1;//RES
	P_LCD_DC  = 1;//DC
	P_LCD_CS  = 1;//CS

	P_LCD_RST = 0;
	delay_ms(100);
	P_LCD_RST = 1;
	delay_ms(100);

	LCD_WR_REG(0x11); 	//Sleep out 退出睡眠
	delay_ms(120);		//Delay 120ms
	
//------------------------------------ST7735S Frame rate-------------------------------------------------//
    LCD_WR_REG(0xB1);       //Frame rate 80Hz
    LCD_WR_DATA8(0x05);  //02
    LCD_WR_DATA8(0x3C);  //35
    LCD_WR_DATA8(0x3C);  //36
    LCD_WR_REG(0xB2);       //Frame rate 80Hz
    LCD_WR_DATA8(0x05); //02
    LCD_WR_DATA8(0x3C); //35
    LCD_WR_DATA8(0x3C); //36
    LCD_WR_REG(0xB3);       //Frame rate 80Hz
    LCD_WR_DATA8(0x05); //02
    LCD_WR_DATA8(0x3C); //35
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    //------------------------------------End ST7735S Frame rate-------------------------------------------//
    LCD_WR_REG(0xB4);      //Dot inversion
    LCD_WR_DATA8(0x03);
    LCD_WR_REG(0x3a);     // Interface pixel format
	//LCD_WR_DATA8(0x03);    // 12-bit/pixel RGB 4-4-4 (4k colors)
	LCD_WR_DATA8(0x05);    // 16-bit/pixel RGB 5-6-5 (65k colors)
	LCD_WR_DATA8(0x06);    // 18-bit/pixel RGB 6-6-6 (256k colors)
    //------------------------------------ST7735S Power Sequence-----------------------------------------//
    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0xA2);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x84);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0xC5);
    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x2A);
    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);
    //---------------------------------End ST7735S Power Sequence---------------------------------------//
    LCD_WR_REG(0xC5);                                      //VCOM
    LCD_WR_DATA8(0x0a);
    LCD_WR_REG(0x36);                                      //这里设定扫描方向
    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
    else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);          //本模组选0XC8
    else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
    else LCD_WR_DATA8(0xA8);
    //---------------------------------ST7735S Gamma Sequence-----------------------------------------//
     LCD_WR_REG(0XE0);
     LCD_WR_DATA8(0x12);
     LCD_WR_DATA8(0x1C);
     LCD_WR_DATA8(0x10);
     LCD_WR_DATA8(0x18);
     LCD_WR_DATA8(0x33);
     LCD_WR_DATA8(0x2C);
     LCD_WR_DATA8(0x25);
     LCD_WR_DATA8(0x28);
     LCD_WR_DATA8(0x28);
     LCD_WR_DATA8(0x27);
     LCD_WR_DATA8(0x2F);
     LCD_WR_DATA8(0x3C);
     LCD_WR_DATA8(0x00);
     LCD_WR_DATA8(0x03);
     LCD_WR_DATA8(0x03);
     LCD_WR_DATA8(0x10);
     LCD_WR_REG(0XE1);
     LCD_WR_DATA8(0x12);
     LCD_WR_DATA8(0x1C);
     LCD_WR_DATA8(0x10);
     LCD_WR_DATA8(0x18);
     LCD_WR_DATA8(0x2D);
     LCD_WR_DATA8(0x28);
     LCD_WR_DATA8(0x23);
     LCD_WR_DATA8(0x28);
     LCD_WR_DATA8(0x28);
     LCD_WR_DATA8(0x26);
     LCD_WR_DATA8(0x2F);
     LCD_WR_DATA8(0x3B);
     LCD_WR_DATA8(0x00);
     LCD_WR_DATA8(0x03);
     LCD_WR_DATA8(0x03);
     LCD_WR_DATA8(0x10);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
    LCD_WR_REG(0x20);     // Display inversion off
  //LCD_WR_REG(0x21);     // Display inversion on
    LCD_WR_REG(0x13);     // Partial mode off
    LCD_WR_REG(0x2A);  //Column addr set
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x7F);
    LCD_WR_REG(0x2B);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x7F);
//    LCD_WR_REG(0x21);                  //显示颜色异常时，打开这条指令
    LCD_WR_REG(0x29); //Display on
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{
	u16 i,j;
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA16(color);
		}
	}
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置
	LCD_WR_DATA16(color);
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;

	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++)
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))		LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++)
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++)
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++)
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置
	for(i=0;i<TypefaceNum;i++)
	{
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))	LCD_WR_DATA16(fc);
				else LCD_WR_DATA16(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}
}


/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
			 if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*p!='\0')
	{
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}

/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10, (u8)(len-t-1)))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+t*sizex,y, (u8)(temp+48),fc,bc,sizey,0);
	}
}

/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10, (u8)(len-t-1)))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,(u8)(temp+48),fc,bc,sizey,0);
	}
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width, u8 xdata *pic)
{
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	SPI_DMA_TRIG(pic);	//触发SPI DAM发送一个图片
	while(B_SPI_DMA_busy);	//等待图片发送完毕
}

//DMA_SPI_CR 	SPI_DMA控制寄存器
#define		DMA_ENSPI		(1<<7)	// SPI DMA功能使能控制位，    bit7, 0:禁止SPI DMA功能，  1：允许SPI DMA功能。
#define		SPI_TRIG_M		(1<<6)	// SPI DMA主机模式触发控制位，bit6, 0:写0无效，          1：写1开始SPI DMA主机模式操作。
#define		SPI_TRIG_S		(0<<5)	// SPI DMA从机模式触发控制位，bit5, 0:写0无效，          1：写1开始SPI DMA从机模式操作。
#define		SPI_CLRFIFO		1		// 清除SPI DMA接收FIFO控制位，bit0, 0:写0无效，          1：写1复位FIFO指针。

//DMA_SPI_CFG 	SPI_DMA配置寄存器
#define		DMA_SPIIE	(1<<7)	// SPI DMA中断使能控制位，bit7, 0:禁止SPI DMA中断，     1：允许中断。
#define		SPI_ACT_TX	(1<<6)	// SPI DMA发送数据控制位，bit6, 0:禁止SPI DMA发送数据，主机只发时钟不发数据，从机也不发. 1：允许发送。
#define		SPI_ACT_RX	(0<<5)	// SPI DMA接收数据控制位，bit5, 0:禁止SPI DMA接收数据，主机只发时钟不收数据，从机也不收. 1：允许接收。
#define		DMA_SPIIP	(0<<2)	// SPI DMA中断优先级控制位，bit3~bit2, (最低)0~3(最高).
#define		DMA_SPIPTY	0		// SPI DMA数据总线访问优先级控制位，bit1~bit0, (最低)0~3(最高).

//DMA_SPI_CFG2 	SPI_DMA配置寄存器2
#define		SPI_WRPSS	(0<<2)	// SPI DMA过程中使能SS脚控制位，bit2, 0: SPI DMA传输过程不自动控制SS脚。  1：自动拉低SS脚。
#define		SPI_SSS	    3		// SPI DMA过程中自动控制SS脚选择位，bit1~bit0, 0: P1.4,  1：P2.4,  2: P4.0,  3:P3.5。

//DMA_SPI_STA 	SPI_DMA状态寄存器
#define		SPI_TXOVW	(1<<2)	// SPI DMA数据覆盖标志位，bit2, 软件清0.
#define		SPI_RXLOSS	(1<<1)	// SPI DMA接收数据丢弃标志位，bit1, 软件清0.
#define		DMA_SPIIF	1		// SPI DMA中断请求标志位，bit0, 软件清0.

//HSSPI_CFG  高速SPI配置寄存器
#define		SS_HOLD		(0<<4)	//高速模式时SS控制信号的HOLD时间， 0~15, 默认3. 在DMA中会增加N个系统时钟，当SPI速度为系统时钟/2时执行DMA，SS_HOLD、SS_SETUP和SS_DACT都必须设置大于2的值.
#define		SS_SETUP		3	//高速模式时SS控制信号的SETUP时间，0~15, 默认3. 在DMA中不影响时间，       当SPI速度为系统时钟/2时执行DMA，SS_HOLD、SS_SETUP和SS_DACT都必须设置大于2的值.

//HSSPI_CFG2  高速SPI配置寄存器2
#define		SPI_IOSW	(1<<6)	//bit6:交换MOSI和MISO脚位，0：不交换，1：交换
#define		HSSPIEN		(0<<5)	//bit5:高速SPI使能位，0：关闭高速模式，1：使能高速模式
#define		FIFOEN		(1<<4)	//bit4:高速SPI的FIFO模式使能位，0：关闭FIFO模式，1：使能FIFO模式，使能FIFO模式在DMA中减少13个系统时间。
#define		SS_DACT			3	//bit3~0:高速模式时SS控制信号的DEACTIVE时间，0~15, 默认3, 不影响DMA时间.  当SPI速度为系统时钟/2时执行DMA，SS_HOLD、SS_SETUP和SS_DACT都必须设置大于2的值.

void SPI_DMA_TRIG(u8 xdata *TxBuf)
{
				//@40MHz, Fosc/4, 200字节258us，100字节  130us，50字节66us，N个字节耗时 N*1.280+2 us, 51T一个字节，其中状态机19T, 传输耗时32T.
				//@40MHz, Fosc/2, 200字节177us，100字节 89.5us，50字节46us，N个字节耗时 N*0.875+2 us, 35T一个字节，其中状态机19T, 传输耗时16T.
				//@40MHz, Fosc/2, SPI DMA传输一个字节, FIFO=1, HOLD=0，耗时16+3=19T(0.475us), HOLD=3，耗时16+6=22T(0.55us).
				//@40MHz, Fosc/4, SPI DMA传输一个字节, FIFO=1, HOLD=0，耗时32+3=35T(0.875us), HOLD=3，耗时32+6=38T(0.95us).
	HSSPI_CFG  = SS_HOLD | SS_SETUP;	//SS_HOLD会增加N个系统时钟, SS_SETUP没有增加时钟。驱动OLED 40MHz时SS_HOLD可以设置为0，
	HSSPI_CFG2 = SPI_IOSW | FIFOEN | SS_DACT;	//FIFOEN允许FIFO会减小13个时钟.

	P_LCD_DC  = 1;	//写数据
	P_LCD_CS  = 0;	//片选
	B_SPI_DMA_busy = 1;	//标志SPI-DMA忙，SPI DMA中断中清除此标志，使用SPI DMA前要确认此标志为0

	SPI_TxAddr   = (u16)TxBuf;		//要发送数据的首地址
	DMA_SPI_TXAH = (u8)(SPI_TxAddr >> 8);		//发送地址寄存器高字节
	DMA_SPI_TXAL = (u8)SPI_TxAddr;				//发送地址寄存器低字节
	DMA_SPI_AMTH = (u8)((3200-1)/256);		//设置传输总字节数(高8位),	设置传输总字节数 = N+1
	DMA_SPI_AMT  = (u8)(3200-1);			//设置传输总字节数(低8位).
	DMA_SPI_ITVH = 0;
	DMA_SPI_ITVL = 0;
	DMA_SPI_STA  = 0x00;
	DMA_SPI_CFG  = DMA_SPIIE | SPI_ACT_TX | SPI_ACT_RX | DMA_SPIIP | DMA_SPIPTY;
	DMA_SPI_CFG2 = SPI_WRPSS | SPI_SSS;
	DMA_SPI_CR   = DMA_ENSPI | SPI_TRIG_M | SPI_TRIG_S | SPI_CLRFIFO;	//启动SPI DMA发送命令
}

//========================================================================
// 函数: void SPI_DMA_ISR (void) interrupt DMA_SPI_VECTOR
// 描述:  SPI_DMA中断函数.
// 参数: none.
// 返回: none.
//========================================================================
void SPI_DMA_ISR (void) interrupt DMA_SPI_VECTOR
{
	DMA_SPI_CR = 0;			//关闭SPI DMA
	B_SPI_DMA_busy = 0;		//清除SPI-DMA忙标志，SPI DMA中断中清除此标志，使用SPI DMA前要确认此标志为0
	SPSTAT = 0x80 + 0x40;	//清0 SPIF和WCOL标志
	HSSPI_CFG2 = SPI_IOSW | SS_DACT;	//使用SPI查询或中断方式时，要禁止FIFO
	P_LCD_CS = 1;
	DMA_SPI_STA = 0;		//清除中断标志
}

/*********************************************************************
 * @fn      nLcd_init
 *
 * @brief   lcd初始化
 *
 * @return  none
 */
void nLcd_init(void)
{
	//==================== SPI初始化 ==================================
	SPI_Config(3, 3);	//(SPI_io, SPI_speed), 参数: 	SPI_io: 切换IO(SS MOSI MISO SCLK), 0: 切换到P1.4 P1.5 P1.6 P1.7,  1: 切换到P2.4 P2.5 P2.6 P2.7, 2: 切换到P4.0 P4.1 P4.2 P4.3,  3: 切换到P3.5 P3.4 P3.3 P3.2,
						//								SPI_speed: SPI的速度, 0: fosc/4,  1: fosc/8,  2: fosc/16,  3: fosc/2
	HSSPI_CFG2 = 0x40;	//交换MOSI MISO, P3.3是MOSI

	P1n_standard(Pin1);			//SPI引脚设置为准双向口, SPI和控制信号
	PullUpEnable(P1PU, Pin1);	// 允许端口内部上拉电阻     PxPU, 要设置的端口对应位为1
	P3n_standard(0x2c);			//SPI引脚设置为准双向口, SPI和控制信号
	PullUpEnable(P3PU, 0x2c);	// 允许端口内部上拉电阻     PxPU, 要设置的端口对应位为1
	P4n_standard(Pin7);			//SPI引脚设置为准双向口, SPI和控制信号
	PullUpEnable(P4PU, Pin7);	// 允许端口内部上拉电阻     PxPU, 要设置的端口对应位为1
	//=================================================================

	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
}