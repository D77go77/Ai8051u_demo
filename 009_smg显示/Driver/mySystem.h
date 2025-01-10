#ifndef MY_SYSTEM_H
#define MY_SYSTEM_H

#include "common.h"

extern unsigned long int uwTick;

void Systick_Init(void);
void led_scan(void);

#endif
