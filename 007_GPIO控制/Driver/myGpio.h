#ifndef MY_GPIO_H
#define MY_GPIO_H

#include "common.h"

typedef struct                   //�ṹ�����
{
    uint32_t pin;                // 4�ֽ� - GPIO���ź�
    uint32_t period;             // 4�ֽ� - ��˸����
    uint16_t times;              // 2�ֽ� - Ԥ����˸�ܴ���
    uint16_t cnt;                // 2�ֽ� - ��˸���Ƽ�����
    uint16_t times_cnt;          // 2�ֽ� - ��¼����˸����
    uint8_t reset;               // 1�ֽ� - ��˸���̸�λ��־
    uint8_t end;                 // 1�ֽ� - ��˸��ɱ�־λ
    float light_on_percent;      // 4�ֽ� - ���������ڵ���ʱ��ٷֱ�
} _gpio_control;

extern _gpio_control led0;
extern _gpio_control led1;

void nGpio_init(void);

void task_led1_proc(void);
void task_led0_proc(void);

void led0_setup(uint32_t _period, float _light_on_percent, uint16_t _times);
void led1_setup(uint32_t _period, float _light_on_percent, uint16_t _times);

#endif
