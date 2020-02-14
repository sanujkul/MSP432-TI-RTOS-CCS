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

#define HEX01               (uint8_t)0x01

/**
 * main.c
 */
void main(void)
{
    //STOPPING WATCHDOG TIMER
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		            // stop watchdog timer

	//COMFIGURE GPIO
	P2->DIR = BIT4 | BIT5 | BIT6 | BIT7;                                     //GIVING P1.0 pin OUT Direcrion

	// Initializing SEL reg so that Primary Module function is selected for P2.4, P2.5, P2.6, P2.7
	P2->SEL0 |= BIT4 | BIT5 | BIT6 | BIT7;
	P2->SEL1 &= ~(BIT4 | BIT5 | BIT6 | BIT7);

	//PWM Period for all 4
	TIMER_A0->CCR[0] = COUNT1;                                  //PWM Period

	//For P2.7
	TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_3; // CCR4 reset/set
	TIMER_A0->CCR[4] = COUNT2;                 // CCR4 PWM duty cycle
	//For P2.6
	TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_4; // CCR3 reset/set
	TIMER_A0->CCR[3] = COUNT2;                 // CCR3 PWM duty cycle
	//For P2.5
	TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // CCR2 reset/set
	TIMER_A0->CCR[2] = COUNT2;                 // CCR2 PWM duty cycle
	//For P2.4
	TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_2; // CCR1 reset/set
	TIMER_A0->CCR[1] = COUNT2;                 // CCR1 PWM duty cycle


	TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK | // SMCL
	            TIMER_A_CTL_MC__UP |            // Up mode
	            TIMER_A_CTL_CLR;                // Clear TAR

	while(1){

	}


}
