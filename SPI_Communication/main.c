/*
 * Using UCB0 in 3 pin SPI Mode as Master
 *  P1.5 - UBC0CLK
 *  P1.6 - UBOSIMO
 *  P1.7 - UCBSOMI
 *  Since it is a modification of one of there codes, hence Copyright the messege of TI is attached below
 */

/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "msp.h"
#include <stdint.h>

//Global variables 8 bits:
static uint8_t RXData = 0;
static uint8_t TXData;

/**
 * main.c
 */
void main(void)
{
    volatile uint32_t i;
    uint8_t previousData;

    P1->DIR |= BIT0;                        // Set P1.0 LED as OUTPUT
    P1->OUT &= ~BIT0;                       //Set p1.0 as LOW (LED OFF)

    //1.stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	//2. Setting Pins P1.5, P1.6, P1.7 in SPI mode: S1 = 0, S0 = 1;
	P1->SEL0 |= BIT5 | BIT6 | BIT7;
	P1->SEL1 &= ~(BIT5 | BIT6 | BIT7);

	//3. Setting Control0 Register
	//Put eUSCI state machine in reset. Setting SWRST BIT Enable reset state. eUSCI logic held in reset state.
	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;

	EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST | // Keep eUSCI state in reset state
	        EUSCI_B_CTLW0_MST |             // (Setting Bit11, therefore Setting as SPI master mode)
	        EUSCI_B_CTLW0_SYNC |            //  Setting Bit8, Set as synchronous mode
	        EUSCI_B_CTLW0_CKPL |            // Set clock polarity high
	        EUSCI_B_CTLW0_MSB |             // MSB first
	        EUSCI_B_CTLW0_SSEL__ACLK;       // Seeting clock source as ACLK (ACLK has 32KHz frequency)

	//4. Initializing Baudrate by writing to Baud Rade Control Register
	EUSCI_B0->BRW = 0x01;                   // /2,fBitClock = fBRCLK/(UCBRx). (Pg 939 TRM)

	//5. Disabling the software reset. Releasing Pins for operation
	EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;

	//6. Enabling the interrupt:
	// Enable global interrupt
	__enable_irq();

	// Enable eUSCI_B0 interrupt in NVIC module
	NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

	// Wake up on exit from ISR
	SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

	// Ensures SLEEPONEXIT takes effect immediately
	__DSB();

	TXData = 0x01;                          // Initialize TX data to 0x01

	while(1)
	    {
	        EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag
	        EUSCI_B0->IE |= EUSCI_B__TXIE;    // Enable TX interrupt
	        __sleep();
	        __no_operation();

	        // Check the received data
	        previousData = TXData - 1;
	        if (RXData != (previousData))
	        {
	            // If the Received data is not equal to TXData-1, then
	            // Set P1.0 LED
	            P1->OUT |= BIT0;
	        }
	        else
	        {
	            P1->OUT &= ~BIT0;
	        }

	        for (i = 2000; i > 0; i--);         // Delay before next transmission
	        TXData++;                           // Increment transmit data
	    }
}

// SPI interrupt service routine
void EUSCIB0_IRQHandler(void)
{
    //If Transmitter Interrupt flag TXIFG is set (i.e. TXBUF is Empty)
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)
    {
        //Put the data in TXBUF (TX Buffer)
        EUSCI_B0->TXBUF = TXData;
        //Resetting the Flag to 0
        EUSCI_B0->IE &= ~EUSCI_B__TXIE;


        // Wait till a character is received
        while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));
        RXData = EUSCI_B0->RXBUF; //Get the data from RX Buffer
        // Clear the receive interrupt flag
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG;
    }
}
