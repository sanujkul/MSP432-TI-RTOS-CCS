#define MSP432P4111
#include "msp.h"

#define DIR_OUT_P1_0 0x01
#define OUT_HIGH_P1_0 0x01
#define OUT_LOW_P1_0 0x00
#define CLEAR_P1_0 0xFE

#define BUTTON11 0X02     //P1.1
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P1->DIR = DIR_OUT_P1_0;             //RED LED (P1.0) pin OUTPUT Direction

	//Button1 is connected to P1.1
	//It needs to be PULLED UP!!:)
	P1->OUT = (uint8_t)BUTTON11;                 //Making it HIGH
	P1->REN = (uint8_t)BUTTON11;                 //Enabling the resistor for P1.1

	while(1)
	{
	    while((BUTTON11 & P1->IN) == 0)         //While button is pressed, GLOW THE LED
	    {
	        P1->OUT = P1->OUT | OUT_HIGH_P1_0;
	    }

	    P1->OUT = P1->OUT & CLEAR_P1_0;
	}


}
