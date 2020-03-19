/*
 * In this we will use both semaphores, Red and green will stuck in deadlock
 * AT thhis time IDLE thread will be running (blinking blue)
 *
 * Here redLedBlink has higher priority than green LED.
 * Since we have not used task.sleep(),
 * we have demonstrated using semaphores for execution of both tasks
 */

/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

// Driver header file
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Board file
#include <ti/drivers/Board.h>

extern Semaphore_Handle greenSemaphore, redSemaphore;

//Though we are using Red and green only here;
Void Task_ledController(UArg arg0, UArg arg1);      //Red
Void Task_ledControllerGreen(UArg arg0, UArg arg1); //Green
Void Task_ledControllerBlue(UArg arg0, UArg arg1);  //Blue
int main()
{
    /* Call driver init functions */
    Board_init();

    System_printf("hello world\n");

    //Red LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    //GREEN LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
    //BLUE LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    System_printf("hello world2 \n");

    System_flush();
    BIOS_start();

//    BIOS_exit(0);  /* terminates program and dumps SysMin output */


    return(0);
}

Void Task_ledController(UArg arg0, UArg arg1)
{
    //Task has infinite while loop
    int i =0;
    while(1)
    {
        //FOllowing sentence means that if greenSemaphore did not arrive,
        //instructions below it wont execute
        Semaphore_pend(greenSemaphore, BIOS_WAIT_FOREVER);

        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

        for(i=0; i<1000000; i++){}

        Semaphore_post(redSemaphore);
    }

}


Void Task_ledControllerGreen(UArg arg0, UArg arg1)
{
    int i =0;
    //Task has infinite while loop
    while(1)
    {
        Semaphore_pend(redSemaphore, BIOS_WAIT_FOREVER);

        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);

        for(i=0; i<1000000; i++){}

        Semaphore_post(greenSemaphore);
    }

}

Void Task_ledControllerBlue(UArg arg0, UArg arg1)
{
    //Task has infinite while loop
    while(1)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
        Task_sleep(200);
    }

}
