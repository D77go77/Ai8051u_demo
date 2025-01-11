#include "myAdckey.h"

AdcKeyHandle Adckey; 
/*********************************************************************
 * @fn      Adckey_init
 * @brief   adc������ʼ��
 *
 * @return  none
 */
void Adckey_init(AdcKeyHandle *handle)
{
	P1M0 &= ~0x01; P1M1 |= 0x01; //P10 һ·ADC��������

    ADCTIM = 0x3f;		//���� ADC �ڲ�ʱ��ADC����ʱ�佨�������ֵ
    ADCCFG = 0x2f;		//���� ADC ʱ��Ϊϵͳʱ��/2/16/16
    ADC_CONTR = 0x80;   //ʹ�� ADC ģ��
    // ��ʼ���ṹ�����
    handle->KeyState = 0;
    handle->KeyState1 = 0;
    handle->KeyState2 = 0;
    handle->KeyState3 = 0;//��״̬
    handle->KeyHoldCnt = 0;//�����¼�ʱ
    handle->KeyCode = 0;//���û�ʹ�õļ���, 1~16��Ч
}
//========================================================================
// ����: u16 Get_ADC12bitResult(u8 channel)
// ����: ��ѯ����һ��ADC���.
// ����: channel: ѡ��Ҫת����ADC.
// ����: 12λADC���.
//========================================================================
uint16_t Get_ADC12bitResult(uint8_t channel)	//channel = 0~15
{
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xf0) | channel; 	// ���� ADC ת��ͨ��
    ADC_START = 1;                            	// ���� ADC ת��
    _nop_();_nop_();_nop_();_nop_();

    while (ADC_FLAG == 0); 						// �ȴ� ADC ת�����
    ADC_FLAG = 0;          						// ��� ADC ת����ɱ�־
    return (((uint16_t)ADC_RES << 8) | ADC_RESL);
}
/***************** ADC���̼������ *****************************
��ADC���̷����ںܶ�ʵ�ʲ�Ʒ�����, ��֤�����ȶ��ɿ�, ��ʹ����ʹ�õ���Ĥ,���ܿɿ�.
16����,�����ϸ�������Ӧ��ADCֵΪ (4096 / 16) * k = 256 * k, k = 1 ~ 16, �ر��, k=16ʱ,��Ӧ��ADCֵ��4095.
����ʵ�ʻ���ƫ��,���ж�ʱ�������ƫ��, ADC_OFFSETΪ+-ƫ��, ��ADCֵ�� (256*k-ADC_OFFSET) �� (256*k+ADC_OFFSET)֮��Ϊ����Ч.
���һ����ʱ��,�Ͳ���һ��ADC,����10ms.
Ϊ�˱���żȻ��ADCֵ����, ���߱���ADC���������½�ʱ����, ʹ������3��ADCֵ����ƫ�Χ��ʱ, ADCֵ����Ϊ��Ч.
�����㷨, �ܱ�֤�����ǳ��ɿ�.
**********************************************/
#define ADC_OFFSET  64
// ���� ADC ��������
void CalculateAdcKey(AdcKeyHandle *handle, uint16_t adc)
{
    uint8_t i;
    uint16_t j;

    if (adc < (256 - ADC_OFFSET))
    {
        handle->KeyState = 0;   // ��״̬�� 0
        handle->KeyHoldCnt = 0; // �������¼�ʱ����
    }

    j = 256;
    for (i = 1; i <= 16; i++)
    {
        if ((adc >= (j - ADC_OFFSET)) && (adc <= (j + ADC_OFFSET)))
            break; // �ж��Ƿ���ƫ�Χ��
        j += 256;
    }
    handle->KeyState3 = handle->KeyState2;
    handle->KeyState2 = handle->KeyState1;
    if (i > 16)
    {
        handle->KeyState1 = 0; // ����Ч
    }
    else
    {
        handle->KeyState1 = i; // ����Ч
        if ((handle->KeyState3 == handle->KeyState2) && (handle->KeyState2 == handle->KeyState1) &&
            (handle->KeyState3 > 0) && (handle->KeyState2 > 0) && (handle->KeyState1 > 0))
        {
            if (handle->KeyState == 0) 		// ��һ�μ�⵽				
            {
                handle->KeyCode = i;  		// �������
                handle->KeyState = i; 		// �����״̬
                handle->KeyHoldCnt = 0;
            }
            if (handle->KeyState == i) 		// ������⵽ͬһ������//�˴�����ӳ������
            {
                if (++handle->KeyHoldCnt >= 100)// ���� 1 ����� 10 ��ÿ����ٶ� Repeat Key
                {
                    handle->KeyHoldCnt = 90;	
                    handle->KeyCode = i; 		// �������
                }
            }
            else
            {
                handle->KeyHoldCnt = 0; // ����ʱ������� 0
            }
        }
    }
}
/*********************************************************************
 * @fn      Adckey_scan
 * @time    10ms
 * @brief   adc����ɨ�����
 *
 * @return  none
 */
// ADC ����ɨ��
void Adckey_scan(AdcKeyHandle *handle)
{
	//����0~15,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 4096 Ϊ����
    uint16_t adc_value = Get_ADC12bitResult(0); // ��ȡͨ�� 0 �� ADC ���
    if (adc_value < 4096)
    {
        CalculateAdcKey(handle, adc_value); // ���㰴��
    }
}
/*********************************************************************
 * @fn      adc_key_proc
 * @time    10ms
 * @brief   adc����ɨ�����
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
        handle->KeyCode = 0; // �������
    }
}
/*********************************************************************
 * @fn      task_adc_key_proc
 * @time    10ms
 * @brief   adc����������
 *
 * @return  none
 */
void task_adc_key_proc(void)
{
	adc_key_proc(&Adckey);
}
/*********************************************************************
 * @fn      nADC_init
 * @brief   adc��ʼ������
 *
 * @return  none
 */
void nADC_init(void)
{
	Adckey_init(&Adckey);
}
