#include "myLed.h"



void led_loop(void)
{
	    P10 = 0;		//LED On
        delay_ms(250);
        P10 = 1;		//LED Off

        P11 = 0;		//LED On
        delay_ms(250);
        P11 = 1;		//LED Off

        P12 = 0;		//LED On
        delay_ms(250);
        P12 = 1;		//LED Off

        P13 = 0;		//LED On
        delay_ms(250);
        P13 = 1;		//LED Off

        P14 = 0;		//LED On
        delay_ms(250);
        P14 = 1;		//LED Off

        P15 = 0;		//LED On
        delay_ms(250);
        P15 = 1;		//LED Off

        P16 = 0;		//LED On
        delay_ms(250);
        P16 = 1;		//LED Off

        P17 = 0;		//LED On
        delay_ms(250);
        P17 = 1;		//LED Off
}


