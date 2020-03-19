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
#include <xdc/runtime/Error.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/hal/Hwi.h>


// Driver header file
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Board file
#include <ti/drivers/Board.h>

#define TimerA1_INTERRUPT_NUMBER 26

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
Swi_Handle swi0;

void ledControl(UArg arg0);
void ledFollowup(UArg arg0, UArg arg1);
/*
 *  ======== main ========
 */
int main()
{
    System_printf("hello world\n");
    /* Call driver init functions */
    Board_init();

    //Creating dynamic Hwi
    Hwi_Handle hwi0;
    Hwi_Params hwiParams;
    Error_Block eb;
    Error_init(&eb);


    Swi_Params swiParams;
    Error_Block eb1;
    Error_init(&eb1);

    Hwi_Params_init(&hwiParams);
    hwi0 = Hwi_create(TimerA1_INTERRUPT_NUMBER,ledControl,&hwiParams,&eb);
    if(hwi0 == NULL){
        System_abort("Hwi_ create fail");
    }

    Swi_Params_init(&swiParams);
    swi0 = Swi_create(ledFollowup, &swiParams, &eb1);

    //Initializing RED LED P1.0
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring Timer_A1 for Up Mode */
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    /* Enabling interrupts and starting the timer */
//    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_TA1_0);    //NVIC = 10, interrupt number = 10+16 = 26
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();


    System_printf("hello world2 \n");



    System_flush();
    BIOS_start();

    return(0);
}

void ledControl(UArg arg0){
    System_printf("In ISR \n");

    Swi_post(swi0);

    //Clear the timer interrupt flag:
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                                             TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void ledFollowup(UArg arg0, UArg arg1){
    System_printf("In Swi Routine \n");
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
