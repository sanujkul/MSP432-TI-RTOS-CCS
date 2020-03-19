
/*
 * Creating 3 tasks and playing with the priorities:
 */

/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>


// Driver header file
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Board file
#include <ti/drivers/Board.h>

Void Task_ledController(UArg arg0, UArg arg1);
Void Task_ledControllerBlue(UArg arg0, UArg arg1);
Void Task_ledControllerGreen(UArg arg0, UArg arg1);

int main()
{
    /* Call driver init functions */
    Board_init();
    //Red LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    //BLUE LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
    //GREEN LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);

    System_printf("hello world2 \n");

    System_flush();
    BIOS_start();

//    BIOS_exit(0);  /* terminates program and dumps SysMin output */


    return(0);
}

Void Task_ledController(UArg arg0, UArg arg1)
{
    //Task has infinite while loop
    while(1)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //Because of this sleep, this tasks goes to sleep and other
        //low priority task gets the CPU. If I make task_sleep(0),
        //then this task will never leave CPU and other Tasks wont
        //get the CPU and will not be executed.
        Task_sleep(200);
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

Void Task_ledControllerGreen(UArg arg0, UArg arg1)
{
    //Task has infinite while loop
    while(1)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        Task_sleep(500);
    }

}
