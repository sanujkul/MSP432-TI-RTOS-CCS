/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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

/*
 *  ======== hello.c ========
 */


/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>

// Driver header file
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Board file
#include <ti/drivers/Board.h>

/* Application Defines  */
#define TIMER_PERIOD    0x2DC6

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // 5000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

//Static hardwareInterrupt, dine changes in .cfg file gui. This is done for timerA1 that has interrupt number 26
void hwi_ledController(void);
void swi_ledController(void);

extern Swi_Handle swiTest;

int main()
{
    /* Call driver init functions */
    Board_init();

//    GPIO_init();

    System_printf("hello world\n");
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring Timer_A1 for Up Mode */
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    /* Enabling interrupts and starting the timer */
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_TA1_0);    //NVIC = 10, interrupt number = 10+16 = 26
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    System_printf("hello world2 \n");

    /*
     *  normal BIOS programs, would call BIOS_start() to enable interrupts
     *  and start the scheduler and kick BIOS into gear.  But, this program
     *  is a simple sanity test and calls BIOS_exit() instead.
     */

    System_flush();
    BIOS_start();

//    BIOS_exit(0);  /* terminates program and dumps SysMin output */


    return(0);
}


void hwi_ledController(void){
    System_printf("hello hwi_ledController \n");
    Swi_post(swiTest);                      //Here Hwi tells Swi that we want to use it

    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void swi_ledController(void)
{
    System_printf("hello swi_ledController \n");
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
