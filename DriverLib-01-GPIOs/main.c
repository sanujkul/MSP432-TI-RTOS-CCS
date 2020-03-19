/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P4111
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 * Author: Sanuj Kulshrestha
 * In This example we will play around with GPIOs using DriverLib of MSP432
 * #1. 1Data will be read from switch and will be written to RED LED
 * 1. Red LED (P1.0) is normal output
 * 2. Switch S1 (P1.1) is normal input. When pressed logic is 0. Therefore pulled up
 *
 *#2. Switch S2 has enabled interrupt. If HIGH -> LOW transition, toggle blue LED
 *
 *
 *
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void switchS2_ISR(){
    //Some delay for switch debouncing
    int i=0;
    for(i=0; i<20000; i++);
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
    //Before leaving interrupt do this:
    //Clear the interrupt flag else, code will repeatedly call this ISR.
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
}

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    //#1. Making RED LED P1.0 as output;
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    //#2. Make Blue LED OUTPUT and switch S2 as input
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    //Enablng interrupt:
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);
    MAP_GPIO_registerInterrupt(GPIO_PORT_P1, &switchS2_ISR);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    while(1)
    {
        //#1. Read from Switch s1 (P1.1) and set the output on LED1 (P1.0)
        uint_fast16_t in = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
        if(in == 0){
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }else{
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    }
}
