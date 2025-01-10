/*
 * key.c
 *
 *  Created on: 2024年12月21日
 *      Author: 19816
 */
#include "myKey.h"
#include <AI8051U.H>

my_key key;

#define K0 P32
#define K1 P33

//User
/*********************************************************************
 * @fn      key_init
 *
 * @brief   按键初始化
 *
 * @return  none
 */
void nKey_init(void)
{
	K0 = K1 = 1;
    key.state[KEY1].pin         = K0;
    key.state[KEY0].value       = 1;
    key.state[KEY0].last_value  = 1;
	
    key.state[KEY1].pin         = K1;
    key.state[KEY1].value       = 1;
    key.state[KEY1].last_value  = 1;

}
/*********************************************************************
 * @fn      read_keyState_scan
 * @time    5ms
 * @brief   按键扫描控制
 *
 * @return  none
 */
void read_keyState_scan(key_state *button,uint16_t _key_num)
{
//    button->value = (GPIO_ReadInputDataBit(button->port, button->pin) != 0 ? 0x01 : 0x00);//弱上拉按键

	switch(_key_num)
	{
		case 0:button->value = (K0 == 0 ? 0x01 : 0x00);break;
		case 1:button->value = (K1 == 0 ? 0x01 : 0x00);break;
	}
	
    if(button->value==0)
    {
        //printf("hello\r\n");
        if(button->last_value!=0)//首次按下
        {

            button->press_time=uwTick;//记录按下的时间点
            button->in_time=uwTick;   //记录按下的时间点
            button->in_press_cnt=0;
        }
        else
        {
            if(uwTick-button->press_time>KEEP_LONG_PRESS_LIMIT)//达到持续长按时间限制，声音提示可以松开了
            {
                //beep
            }
            else if(uwTick-button->in_time>IN_PRESS_LIMIT)//持续按下
            {
                button->in_time=uwTick;
                button->press=IN_PRESS;
                if(button->press==IN_PRESS)  button->in_press_cnt++;
            }
        }
    }
    else
    {
        if(button->last_value==0)//按下后释放
        {
            button->release_time=uwTick;//记录释放时的时间
            if(button->release_time-button->press_time>KEEP_LONG_PRESS_LIMIT)//持续长按按键5S
            {
                button->press=KEEP_LONG_PRESS;
                button->state_lock_time=0;
            }
            else if(button->release_time-button->press_time>LONG_PRESS_LIMIT)//长按按键1S
            {
                button->press=LONG_PRESS;
                button->state_lock_time=0;//5ms*300=1.5S
            }
            else
            {
              button->press=SHORT_PRESS;
                button->state_lock_time=0;//5ms*300=1.5S
            }
        }
    }

    button->last_value=button->value;

    if(button->press==LONG_PRESS||button->press==SHORT_PRESS)//按下释放后，程序后台1.5S内无响应，复位按键状态//防止程序卡顿问题
    {
        button->state_lock_time++;
        if(button->state_lock_time>=300)//300*5ms=1.5s
        {
             button->press=NO_PRESS;
             button->state_lock_time=0;
        }
    }
}

/*********************************************************************
 * @fn      key_proc
 * @time    5ms
 * @brief   key控制函数
 *
 * @return  none
 */

void key_proc(void)
{
    if(key.state[KEY0].press==SHORT_PRESS)
    {
        key.state[KEY0].press=NO_PRESS;

        led0_setup(20, 0.3, 5);//200*5
    }
    if(key.state[KEY1].press==SHORT_PRESS)
    {
        key.state[KEY1].press=NO_PRESS;


        led1_setup(20, 0.8, 5);//500*5
    }
}

/*********************************************************************
 * @fn      task_key_gets
 * @time    5ms
 * @brief   key任务扫描函数
 *
 * @return  none
 */
void task_key_gets(void)                //5ms
{
	uint16_t i;
    for(i=0;i<KEY_NUM;i++)
    {
        read_keyState_scan(&key.state[i],i);
    }
    key_proc();
}











