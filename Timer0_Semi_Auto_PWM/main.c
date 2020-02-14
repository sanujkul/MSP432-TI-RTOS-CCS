#define MSP432P4111
#include "msp.h"

//FOR LED at P1.0
#define DIR_OUT_P1_0        0x01
#define OUT_HIGH_P1_0       0x01
#define OUT_LOW_P1_0        0x00

//FOR Timer_A0
#define UP                  0x0010
#define ACLK                0x0100

#define COUNT1              50000
#define COUNT2              45000

#define TIMER_A_CCTL_CCIG   0x0001

/**//************************************************************************
 * main.c
 *///************************************************************************
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             // stop watchdog timer

    P1->DIR = DIR_OUT_P1_0;                                 //P1.0 (RED LED) is OUT mode, rest of P1 are inputs
    P1->OUT = OUT_LOW_P1_0;                                 //P1.0 -> LOW

    TIMER_A0->CTL = UP | ACLK;                              //Source clock is ACLK (default = 32kHz for MSP432) and count UP to TA0CCR0

    TIMER_A0->CCR[0] = COUNT1;                               //Count 0-->50K then start at 0 again
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;                 // CCR0 interrupt triggers when count equals TA0->CCR[0] (i.e. 50k)

    TIMER_A0->CCR[1] = COUNT2;                               //Setting up count in Capture/Compare register 1
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;                 // CCR0 interrupt triggers when count equals TA0->CCR[1] (i.e. 45k)

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);

    __enable_irq();                                     // Enable global interrupts

    while(1){
        printf("\tInfi_While_Loop\n");
    }
}

//************************************************************************
// Timer0 Interrupt Service Routine when COUNT reaches TA0CCR1 (COUNT2)
//************************************************************************
void TA0_N_IRQHandler(void)
{
    printf("N_IRQHandler\n");                                             //Conclusion: Maybe this Hello is helping in Debouncing

    if(TIMER_A0->IV == 2)                               //Timer_A0->IV (Timer A0 -> Inerrupt Vector) has a value of 2 when there is a TA0CCR1 match.
    {
        P1->OUT = OUT_HIGH_P1_0;                        //Then turn on RED LED when COUNT is 45k
    }

    TIMER_A0->CCTL[1] = TIMER_A0->CCTL[1] & (~TIMER_A_CCTL_CCIG); //Resetting the Interrupt flag pin to 0
}

//************************************************************************
// Timer0 Interrupt Service Routine when COUNT reaches TA0CCR0 (COUNT1)
//************************************************************************
void TA0_0_IRQHandler(void)
{
    printf("0_IRQHandler\n");                      //Conclusion: Maybe this Hello is helping in Debouncing
    P1->OUT = OUT_LOW_P1_0;                        //Then turn OFF RED LED when COUNT is reaches TA->CCR[0]
    TIMER_A0->CCTL[0] = TIMER_A0->CCTL[0] & (~TIMER_A_CCTL_CCIG); //Resetting the Interrupt flag pin to 0
}
