//Challenge1: 20s Delay. COUNT = 64k, interval = 10 for 20s time.
//Challlenge2: 3s ON, 1s OFF.


#define MSP432P4111
#include "msp.h"

//FIR LED at P1.0
#define DIR_OUT_P1_0 0x01
#define OUT_HIGH_P1_0 0x01
#define OUT_LOW_P1_0 0x00


//FOR Timer_A0
#define UP 0x0010
#define ACLK 0x0100
#define COUNT 32000
/**
 * main.c
 */
void main(void)
{
    //Setting up (stopping) Watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //Setting up Timer_A0 to count to 40000 in UP Mode
    TIMER_A0->CTL = (UP | ACLK);
    TIMER_A0->CCR[0] = (uint16_t)COUNT;

    //Setting up RED LED
    P1->DIR = DIR_OUT_P1_0;             //RED LED (P1.0) pin OUTPUT Direction

    unsigned char intervals = 0;
    while(1)                            //Infinte while loop
    {
        if(TIMER_A0->CTL & TIMER_A_CTL_IFG) //Now checking if Timer has counted to COUNT or not
        {
            intervals = intervals + 1;
            //Next, we need to make sure that the general purpose timer keeps going
            //Count again. Again setting up Timer A0 CNTRL Register and clear Timer A0 Interrupt Flag Bit.
            TIMER_A0->CTL = TIMER_A0->CTL & (~TIMER_A_CTL_IFG); //COUNT AGAIN

            //Challenge 1: 20 s
            //If we have reached 10 intervals of 64,000 COUNT, i.e. we have counted 640000
//            if(intervals == 10)
//            {
//                intervals = 0;
//                P1->OUT = P1->OUT ^ OUT_HIGH_P1_0; //XOR operation with 00000001 with flip D0 bit;
//            }

            //Challenge2:
            if(intervals == 4)      //TURN LED OFF
            {
                intervals = 0;
                P1->OUT = P1->OUT & (~OUT_HIGH_P1_0); //This will make D0=0, without effecting other bits
            }else {
                P1->OUT = P1->OUT | OUT_HIGH_P1_0;  //This will make D0 = 1, without effecting other bits
            }

        }

    }
}
