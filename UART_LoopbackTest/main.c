
//NOTE: I am doing it for UCA2 (P3.2, P3.3) as UCA0 is also connected to XDSET

//******************************************************************************
//   MSP432P401 Demo - eUSCI_A0 UART External Loopback test @ 115200 baud
//
//  Description: This demo requires an external jumper connection between TX to
//  RX of the eUSCI UART pins. In this demo, the application transmites a
//  character out and loops back to the receive pin thus triggering an
//  interrupt. It then checks if the received data is exactly what is
//  transmitted. If it is not, it sets P1.0 LED ON.
//
//   SMCLK = MCLK = default DCO = 3MHz
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |                 |
//            |     P1.3/UCA0TXD|----
//            |                 |   |
//            |     P1.2/UCA0RXD|----
//            |                 |
//            |            P1.0 |--> LED
//            |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | June 2014 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************

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
 *
 *******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see http://www.ti.com/tool/mspdriverlib for an API functional
 * library & https://dev.ti.com/pinmux/ for a GUI approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/

#include "msp.h"
#include <stdint.h>

uint8_t TXData = 1;

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;                       // P1.0 outout low

    // Configure UART pins
    P3->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as second function

    // Configure UART
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    EUSCI_A2->BRW = 1;                      // Using baud rate calculator
    EUSCI_A2->MCTLW = (10 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Initialize eUSCI
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    //SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Ensures SLEEPONEXIT occurs immediately
    //__DSB();

    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31);

    while (1)
    {
        printf("infi while \n");
        // Arbitrary delay between each transmit to observe the
        // characters being transmitted through a scope
        for (i = 1000; i > 0; i--);

        //while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));

        printf("TxData = %u \n",TXData);
        EUSCI_A2->TXBUF = TXData;           // Load data onto buffer

       // __sleep();
    }
}

// UART interrupt service routine
void EUSCIA2_IRQHandler(void)
{
    printf("ISR\n");

    uint8_t RXData;

    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    {
        EUSCI_A2->IFG &=~ EUSCI_A_IFG_RXIFG;// Clear interrupt
        RXData = EUSCI_A2->RXBUF;           // Clear buffer

        printf("RxData = %u \n",TXData);

        if(RXData == TXData)                // Check value
        {
            printf("Correct Data = Rx = %u, Tx  = %u \n ",RXData,TXData);
            P1->OUT ^= BIT0;                // If correct, toggle on P1.0
            //while(1);                       // trap CPU
        }
        TXData++;                           // increment data byte
    }
}
