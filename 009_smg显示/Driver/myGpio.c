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
 * @brief   实现gpio端口结构体初始化
 *
 * @return  none
 */
void nGpio_init(void)
{
    led0.pin = 0;
    led0.period = 40;                //40*5ms//总200ms脉冲，高电平30%
    led0.light_on_percent = 0.3f;
    led0.reset = 1;
    led0.times = 5;

    led1.pin = 0;
    led1.period = 40;                //40*5ms//总200ms脉冲，高电平80%
    led1.light_on_percent = 0.8f;
    led1.reset = 1;
    led1.times = 5;
	
    beep.pin = 0;
    beep.period = 400;                //40*5ms//总200ms脉冲，高电平80%
    beep.light_on_percent = 0.8f;
    beep.reset = 1;
    beep.times = 5;
}
/*********************************************************************
 * @fn      gpio_control_work(_gpio_control *light)
 * @time    5ms
 * @brief   gpio驱动状态机
 * @input   _gpio_control *light-gpio控制结构体
 * @return  none
 */
void gpio_control_work(_gpio_control *light)
{
    if(light->reset==1)                                    //重置gpio控制
    {
		light->reset=0;
        light->cnt=0;
        light->times_cnt=0;
        light->end=0;
    }
    if(light->times_cnt==light->times)                    //gpio周期控制
	{ 
        light->end=1;
        return;
    }
    light->cnt++;                                           //5ms
    if(light->cnt<=light->period*light->light_on_percent) // 在点亮时间区间内（根据 light_on_percent）
    { 
		light->pin = 1;
    }else if(light->cnt<light->period)                     // 在熄灭时间区间内
    { 
		light->pin = 0;
    }else                                                  //周期结束，重置计数并记录已完成次数
    {    
		light->cnt=0;
        light->times_cnt++;
    }
}
/*********************************************************************
 * @fn      led_setup
 * @time
 * @brief   led及时控制
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
 * @brief   led任务函数
 *
 * @return  none
 */
void task_led0_proc(void)
{
    gpio_control_work(&led0);
	LED0 = !led0.pin;//led倒置高->低
}
void task_led1_proc(void)
{
    gpio_control_work(&led1);
	LED1 = !led1.pin;//led倒置高->低
}
void task_beep_proc(void)
{
	gpio_control_work(&beep);
	BEEP = !beep.pin;
}
