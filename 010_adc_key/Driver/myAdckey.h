#ifndef MY_ADC_KEY_H
#define MY_ADC_KEY_H

#include  "common.h"

// ����һ���ṹ�壬���ڴ洢 ADC ��������ر���
typedef struct
{
    uint8_t KeyState;       // ��ǰ��״̬
    uint8_t KeyState1;      // ���һ�εļ�״̬
    uint8_t KeyState2;      // ǰһ�εļ�״̬
    uint8_t KeyState3;      // ��ǰһ�εļ�״̬
    uint8_t KeyHoldCnt;     // �������¼�ʱ
    uint8_t KeyCode;        // ��ǰ��⵽�ļ��� (1~16 ��Ч)
} AdcKeyHandle;

// ���� ADC ������غ����Ľӿ�

void nADC_init(void);
void task_adc_key_proc(void);


#endif
