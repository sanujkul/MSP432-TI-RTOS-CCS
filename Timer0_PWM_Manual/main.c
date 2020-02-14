#define MSP432P4111
#include "msp.h"

//FOR LED at P1.0
#define DIR_OUT_P1_0        0x01
#define OUT_HIGH_P1_0       0x01
#define OUT_LOW_P1_0        0x00

//FOR Timer_A0
#define UP                  0x0010
#define ACLK                0x0100

#define COUNT1              700
#define COUNT2              300

#define TIMER_A_CCTL_CCIG   0x0001

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             // stop watchdog timer

    P1->DIR = DIR_OUT_P1_0;                                 //P1.0 (RED LED) is OUT mode, rest of P1 are inputs
    P1->OUT = OUT_LOW_P1_0;

    TIMER_A0->CCR[0] = COUNT1;                               //Setting up count in Capture/Compare register 0
    TIMER_A0->CTL = UP | ACLK;                              //Source clock is ACLK (default = 32kHz for MSP432)
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;                 // Enable interrupt for Timer_A0

    //P1->OUT = OUT_HIGH_P1_0;

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

    __enable_irq();                                     // Enable global interrupts
    //__enable_interrupt();                                    // Enable global interrupts
    while(1){
        printf("Infi_While_Loop\n");
    }
}

//************************************************************************
// Timer0 Interrupt Service Routine
//************************************************************************
void TA0_0_IRQHandler(void)
{
//    printf("TA0_0_IRQHandler\n");                                             //Conclusion: Maybe this Hello is helping in Debouncing

    if(TIMER_A0->CCR[0] == COUNT1)                      //If this Interrpt is aused because it was counting to 45k
    {
        printf("HIGH\n");
        P1->OUT = OUT_HIGH_P1_0;                        //Then turn on RED LED
        TIMER_A0->CCR[0] = COUNT2;                      //Now set COUNT to 5k
    }
    else                                                //Else it was interrupted because it was counting to 5k
    {
        printf("LOW\n");
        P1->OUT = OUT_LOW_P1_0;                        //Then turn on RED LED
        TIMER_A0->CCR[0] = COUNT1;                      //Now set COUNT to 45k
    }

    //Next, we need to make sure that the general purpose timer keeps going
    //Count again. Again setting up Timer A0 CNTRL Register and clear Timer A0 Interrupt Flag Bit.
    //TIMER_A0->CTL = TIMER_A0->CTL & (~TIMER_A_CTL_IFG);           //HAVING/NOT HAVING this does not make difference

    TIMER_A0->CCTL[0] = TIMER_A0->CCTL[0] & (~TIMER_A_CCTL_CCIG); //Resetting the Interrupt flag pin to 0
    //TIMER_A0->CTL = TIMER_A0->CTL & (~TIMER_A_CTL_IFG);`        //Resetting this flag will not work as ISR will continuosly be called
}
