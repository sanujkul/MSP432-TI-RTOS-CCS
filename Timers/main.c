#define MSP432P4111
#include "msp.h"

//FIR LED at P1.0
#define DIR_OUT_P1_0 0x01
#define OUT_HIGH_P1_0 0x01
#define OUT_LOW_P1_0 0x00

//FOR Timer_A0
#define UP 0x0010
#define ACLK 0x0100
#define COUNT 70000
/**
 * main.c
 */
void main(void)
{
    //Setting up (stopping) Watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Setting up Timer_A0 to count to 40000 in UP Mode
	TIMER_A0->CTL = (UP | ACLK);
	TIMER_A0->CCR[0] = (uint16_t)COUNT;

	//Setting up RED LED
	P1->DIR = DIR_OUT_P1_0;             //RED LED (P1.0) pin OUTPUT Direction

	while(1)                            //Infinte while loop
	{
	    if(TIMER_A0->CTL & TIMER_A_CTL_IFG) //Now checking if Timer has counted to COUNT or not
	    {
	        P1->OUT = P1->OUT ^ OUT_HIGH_P1_0; //XOR operation with 00000001 with flip D0 bit;
	        //Next, we need to make sure that the general purpose timer keeps going
	        //Count again. Again setting up Timer A0 CNTRL Register and clear Timer A0 Interrupt Flag Bit.
	        TIMER_A0->CTL = TIMER_A0->CTL & (~TIMER_A_CTL_IFG);

	    }

	}
}
