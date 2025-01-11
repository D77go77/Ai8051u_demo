#include "myGpio.h"

#define LED0 P00
#define LED1 P01
#define BEEP P50

_gpio_control  led0;
_gpio_control  led1;
_gpio_control  beep;
/*********************************************************************
 * @fn      nGpio_init
 *
 * @brief   ʵ��gpio�˿ڽṹ���ʼ��
 *
 * @return  none
 */
void nGpio_init(void)
{
    led0.pin = 0;
    led0.period = 40;                //40*5ms//��200ms���壬�ߵ�ƽ30%
    led0.light_on_percent = 0.3f;
    led0.reset = 1;
    led0.times = 5;

    led1.pin = 0;
    led1.period = 40;                //40*5ms//��200ms���壬�ߵ�ƽ80%
    led1.light_on_percent = 0.8f;
    led1.reset = 1;
    led1.times = 5;
	
    beep.pin = 0;
    beep.period = 400;                //40*5ms//��200ms���壬�ߵ�ƽ80%
    beep.light_on_percent = 0.8f;
    beep.reset = 1;
    beep.times = 5;
}
/*********************************************************************
 * @fn      gpio_control_work(_gpio_control *light)
 * @time    5ms
 * @brief   gpio����״̬��
 * @input   _gpio_control *light-gpio���ƽṹ��
 * @return  none
 */
void gpio_control_work(_gpio_control *light)
{
    if(light->reset==1)                                    //����gpio����
    {
		light->reset=0;
        light->cnt=0;
        light->times_cnt=0;
        light->end=0;
    }
    if(light->times_cnt==light->times)                    //gpio���ڿ���
	{ 
        light->end=1;
        return;
    }
    light->cnt++;                                           //5ms
    if(light->cnt<=light->period*light->light_on_percent) // �ڵ���ʱ�������ڣ����� light_on_percent��
    { 
		light->pin = 1;
    }else if(light->cnt<light->period)                     // ��Ϩ��ʱ��������
    { 
		light->pin = 0;
    }else                                                  //���ڽ��������ü�������¼����ɴ���
    {    
		light->cnt=0;
        light->times_cnt++;
    }
}
/*********************************************************************
 * @fn      led_setup
 * @time
 * @brief   led��ʱ����
 * @input   uint32_t _period, float _light_on_percent, uint16_t _times
 * @return  none
 */
void led0_setup(uint32_t _period, float _light_on_percent, uint16_t _times)
{
    led0.period = _period; //x*1ms
    led0.light_on_percent = _light_on_percent;
    led0.reset = 1;
    led0.times = _times;
}
void led1_setup(uint32_t _period, float _light_on_percent, uint16_t _times)
{
    led1.period = _period; //x*1ms
    led1.light_on_percent = _light_on_percent;
    led1.reset = 1;
    led1.times = _times;
}
void beep_setup(uint32_t _period, float _light_on_percent, uint16_t _times)
{
    beep.period = _period; //x*1ms
    beep.light_on_percent = _light_on_percent;
    beep.reset = 1;
    beep.times = _times;
}
/*********************************************************************
 * @fn      task_led_proc
 * @time    5ms
 * @brief   led������
 *
 * @return  none
 */
void task_led0_proc(void)
{
    gpio_control_work(&led0);
	LED0 = !led0.pin;//led���ø�->��
}
void task_led1_proc(void)
{
    gpio_control_work(&led1);
	LED1 = !led1.pin;//led���ø�->��
}
void task_beep_proc(void)
{
	gpio_control_work(&beep);
	BEEP = !beep.pin;
}