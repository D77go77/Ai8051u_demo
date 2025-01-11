/*
 * key.h
 *
 *  Created on: 2024年12月21日
 *      Author: 19816
 */

#ifndef MYDRIVER_MYKEY_H_
#define MYDRIVER_MYKEY_H_

#include "common.h"

typedef enum {
    KEY0=0,
	KEY1,
    KEY_NUM                            //按键数量
}KEY;

typedef enum
{
    NO_PRESS=0,
    SHORT_PRESS,
    LONG_PRESS,
    IN_PRESS,
    KEEP_LONG_PRESS,
}press_state;

typedef struct {
    press_state    press;            // 当前按键状态
    uint8_t        value;            // 当前按键值
    uint8_t        last_value;       // 上一次按键值

    uint32_t       pin;              // 按键 GPIO 引脚号
    uint32_t       press_time;       // 按下时间戳
    uint32_t       release_time;     // 松开时间戳
    uint32_t       in_time;          // 按住持续时间
    uint32_t       in_press_cnt;     // 按住计数
    uint32_t       state_lock_time;  // 状态锁定时间
} key_state;

typedef struct
{
    key_state state[KEY_NUM];          // 最大按键位
}my_key;


#define LONG_PRESS_LIMIT        1000 //1000ms
#define IN_PRESS_LIMIT          200  //250ms
#define KEEP_LONG_PRESS_LIMIT   5000 //长按限制5S

void nKey_init(void);
void task_key_gets(void);
void key_proc(void);

extern my_key key;

#endif /* MYDRIVER_MYKEY_H_ */
