# MSP432P4111-CCS
These are Code Composer Studio projects for TI's MSP432P4111 Launchpad

### How can you check or run these projects?  
1. Either you can import whole project on your CCS.  
2. Or you can check out the main.c file in all the projects.  

## Projects explaination:  
### 1. Blink. 
This program plays with the onboard LED. It will turn on Onboard red LED (connected to P1.0) and switched RGB Led color in the sequence Red (P2.0) ->Green (P2.1) ->Blue (P2.2).  

### 2. PushButton_LED.  
This program demonstrates simple digital I/O functionalities. It reads from the onboard push button (P1.4) and changes the state of red LED (P1.0).  

### 3. Timers.  
This program uses TimerA0 to continuously toggle red led (P1.0) after timer has finished counting.  

### 4. TImer_Up_Long.  
This program shows how to keep count of time that is out of limit where timer can count to (CCR[0] register).   

### 5. TimerInterrupts.  
This program shows initializing timers for interrupt and use of Timer ISR.  

### 6. Timer0_PWM_Manual.   
This program shows how to use Timer to manually generate PWM using Timer ISR.  

### 7. Timer0_Semi_Auto_PWM.  
This program also shows a manual way to generate PWM using Timer. Two ISRs have been used.    

### 8. Timer_All_PWMs
This pograms puts up a PWM signal on all the Pins (that are also port mapepd to Timers) and are available as peripheral pin on the launchpad.

### 9. UART_LoopbackTest.  
This program was tested by either connected Tx to Rx or connecting to Rx,Tx of arduino and programming arduino to send back the data it recieved. The value to data sent by MSP increases if it recieves correct data. This program also shows the reciever ISR's functionalities.  The codeflow (what this software does) was inspired form TI example codes for other launchpad.

