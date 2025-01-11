#include "myAdckey.h"

AdcKeyHandle Adckey; 
/*********************************************************************
 * @fn      Adckey_init
 * @brief   adc按键初始化
 *
 * @return  none
 */
void Adckey_init(AdcKeyHandle *handle)
{
	P1M0 &= ~0x01; P1M1 |= 0x01; //P10 一路ADC高阻输入

    ADCTIM = 0x3f;		//设置 ADC 内部时序，ADC采样时间建议设最大值
    ADCCFG = 0x2f;		//设置 ADC 时钟为系统时钟/2/16/16
    ADC_CONTR = 0x80;   //使能 ADC 模块
    // 初始化结构体变量
    handle->KeyState = 0;
    handle->KeyState1 = 0;
    handle->KeyState2 = 0;
    handle->KeyState3 = 0;//键状态
    handle->KeyHoldCnt = 0;//键按下计时
    handle->KeyCode = 0;//给用户使用的键码, 1~16有效
}
//========================================================================
// 函数: u16 Get_ADC12bitResult(u8 channel)
// 描述: 查询法读一次ADC结果.
// 参数: channel: 选择要转换的ADC.
// 返回: 12位ADC结果.
//========================================================================
uint16_t Get_ADC12bitResult(uint8_t channel)	//channel = 0~15
{
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xf0) | channel; 	// 设置 ADC 转换通道
    ADC_START = 1;                            	// 启动 ADC 转换
    _nop_();_nop_();_nop_();_nop_();

    while (ADC_FLAG == 0); 						// 等待 ADC 转换完成
    ADC_FLAG = 0;          						// 清除 ADC 转换完成标志
    return (((uint16_t)ADC_RES << 8) | ADC_RESL);
}
/***************** ADC键盘计算键码 *****************************
本ADC键盘方案在很多实际产品设计中, 验证了其稳定可靠, 即使按键使用导电膜,都很可靠.
16个键,理论上各个键对应的ADC值为 (4096 / 16) * k = 256 * k, k = 1 ~ 16, 特别的, k=16时,对应的ADC值是4095.
但是实际会有偏差,则判断时限制这个偏差, ADC_OFFSET为+-偏差, 则ADC值在 (256*k-ADC_OFFSET) 与 (256*k+ADC_OFFSET)之间为键有效.
间隔一定的时间,就采样一次ADC,比如10ms.
为了避免偶然的ADC值误判, 或者避免ADC在上升或下降时误判, 使用连续3次ADC值均在偏差范围内时, ADC值才认为有效.
以上算法, 能保证读键非常可靠.
**********************************************/
#define ADC_OFFSET  64
// 计算 ADC 按键键码
void CalculateAdcKey(AdcKeyHandle *handle, uint16_t adc)
{
    uint8_t i;
    uint16_t j;

    if (adc < (256 - ADC_OFFSET))
    {
        handle->KeyState = 0;   // 键状态归 0
        handle->KeyHoldCnt = 0; // 按键按下计时清零
    }

    j = 256;
    for (i = 1; i <= 16; i++)
    {
        if ((adc >= (j - ADC_OFFSET)) && (adc <= (j + ADC_OFFSET)))
            break; // 判断是否在偏差范围内
        j += 256;
    }
    handle->KeyState3 = handle->KeyState2;
    handle->KeyState2 = handle->KeyState1;
    if (i > 16)
    {
        handle->KeyState1 = 0; // 键无效
    }
    else
    {
        handle->KeyState1 = i; // 键有效
        if ((handle->KeyState3 == handle->KeyState2) && (handle->KeyState2 == handle->KeyState1) &&
            (handle->KeyState3 > 0) && (handle->KeyState2 > 0) && (handle->KeyState1 > 0))
        {
            if (handle->KeyState == 0) 		// 第一次检测到				
            {
                handle->KeyCode = i;  		// 保存键码
                handle->KeyState = i; 		// 保存键状态
                handle->KeyHoldCnt = 0;
            }
            if (handle->KeyState == i) 		// 连续检测到同一键按着//此处可添加长按检测
            {
                if (++handle->KeyHoldCnt >= 100)// 按下 1 秒后，以 10 次每秒的速度 Repeat Key
                {
                    handle->KeyHoldCnt = 90;	
                    handle->KeyCode = i; 		// 保存键码
                }
            }
            else
            {
                handle->KeyHoldCnt = 0; // 按下时间计数归 0
            }
        }
    }
}
/*********************************************************************
 * @fn      Adckey_scan
 * @time    10ms
 * @brief   adc按键扫描控制
 *
 * @return  none
 */
// ADC 按键扫描
void Adckey_scan(AdcKeyHandle *handle)
{
	//参数0~15,查询方式做一次ADC, 返回值就是结果, == 4096 为错误
    uint16_t adc_value = Get_ADC12bitResult(0); // 获取通道 0 的 ADC 结果
    if (adc_value < 4096)
    {
        CalculateAdcKey(handle, adc_value); // 计算按键
    }
}
/*********************************************************************
 * @fn      adc_key_proc
 * @time    10ms
 * @brief   adc按键扫描控制
 *
 * @return  none
 */
void adc_key_proc(AdcKeyHandle *handle)
{
    Adckey_scan(handle);
    if (handle->KeyCode > 0)
    {
        switch (handle->KeyCode)
        {
        case 1:
            cj.state = 10;
            break;
        case 2:
            cj.state = 20;
            break;
        case 3:
            cj.state = 30;
            break;
        case 4:
            cj.state = 40;
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
        case 16:
            break;
        }
        handle->KeyCode = 0; // 清除键码
    }
}
/*********************************************************************
 * @fn      task_adc_key_proc
 * @time    10ms
 * @brief   adc按键任务函数
 *
 * @return  none
 */
void task_adc_key_proc(void)
{
	adc_key_proc(&Adckey);
}
/*********************************************************************
 * @fn      nADC_init
 * @brief   adc初始化函数
 *
 * @return  none
 */
void nADC_init(void)
{
	Adckey_init(&Adckey);
}
