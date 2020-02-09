#define MSP432P4111

#include "msp.h"             //This file will include msp432p4111.h file as we have defined __MSP432P4111__

#define DIR_OUT_P1_0 0x01
#define OUT_HIGH_P1_0 0x01
#define OUT_LOW_P1_0 0x00

#define RGBLED_OUTPUT 0x07

#define RGB_LOOP_TIME 180000

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P1->DIR = (uint8_t)DIR_OUT_P1_0;  //seeting P1.0 as output
	P1->OUT = (uint8_t)OUT_HIGH_P1_0; //Making P1.0 HIGH

	P2->DIR = (uint8_t)RGBLED_OUTPUT;
	P2->OUT = (uint8_t)0x02;           //GREEN LED (P2.1) HIGH

	long x = 0;                     //x will be used for delay
	while(1)
	{
	    for(x=0; x<RGB_LOOP_TIME; x=x+1)
	    {
	        if(x < RGB_LOOP_TIME/3)
	            P2->OUT = (uint8_t)0x01;
	        else if(x < (RGB_LOOP_TIME*2/3))
	            P2->OUT = (uint8_t)0x02;
	        else
	            P2->OUT = (uint8_t)0x04;
	    }
	}
}
