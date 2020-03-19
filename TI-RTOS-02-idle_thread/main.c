//We made the use of idle thread using .cfg file. This saved us from writing codes here

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

//We have configured it in .cfg file by add =ing the idle functionality
void ledController(void){
    Task_sleep(100); //100 ms sleep
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
}
/*
 *  ======== main ========
 */
int main()
{

    Task_Params taskParams;

    /* Call driver init functions */
    Board_init();

    System_printf("hello world\n");
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);

    System_flush();
    BIOS_start();

//    BIOS_exit(0);  /* terminates program and dumps SysMin output */


    return(0);
}
