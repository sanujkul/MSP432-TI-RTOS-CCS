#include "msp.h"


/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Configuring P1.0 as output and P1.1 (switch) as input with pull-up
    // resistor. Rest of pins are configured as output low.
    // Notice intentional '=' assignment since all P1 pins are being
    // deliberately configured
    P1->DIR = ~(uint8_t) BIT1;              //~BIT1 = 11111101
    P1->OUT = BIT1;
    P1->REN = BIT1;                         // Enable pull-up resistor (P1.1 output high)
    //General I/O mode:
    P1->SEL0 = 0;
    P1->SEL1 = 0;

    P1->IES = BIT1;                         // Setting the bit so that Interrupt on high-to-low transition
    P1->IFG = 0;                            // Clear all P1 interrupt flags
    P1->IE = BIT1;                          // Enable interrupt for P1.1

    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);\

    // Terminate all remaining pins on the device
    P2->DIR |= 0xFF; P2->OUT = 0;
    P3->DIR |= 0xFF; P3->OUT = 0;
    P4->DIR |= 0xFF; P4->OUT = 0;
    P5->DIR |= 0xFF; P5->OUT = 0;
    P6->DIR |= 0xFF; P6->OUT = 0;
    P7->DIR |= 0xFF; P7->OUT = 0;
    P8->DIR |= 0xFF; P8->OUT = 0;
    P9->DIR |= 0xFF; P9->OUT = 0;
    P10->DIR |= 0xFF; P10->OUT = 0;

    // Enable global interrupt
    __enable_irq();
}

/* Port1 ISR */
void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    // Toggling the output on the LED
    if(P1->IFG & BIT1)
        P1->OUT ^= BIT0;

    // Delay for switch debounce
    for(i = 0; i < 10000; i++)

    P1->IFG &= ~BIT1;                   // Need to manually clear the bit that
                                        // caused the interrupt
                                        // BIT1 = 0000 0010 B
                                        //~BIT1 = 1111 1101 B
                                        //When we AND P1IFG with ~BIT1, the P1IFG.1 bit will be cleared
}
