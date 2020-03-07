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
