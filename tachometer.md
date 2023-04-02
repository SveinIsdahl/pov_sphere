## Tachometer signal
Because the motor is from a PC-fan, the tachometer is built into the motor housing. The signal pulses from the green wire are as follows:

           ┌───────────┐           ┌───────────┐
           │           │           │           │
           │           │           │           │
───────────┘           └───────────┘           └─────────────
  Start      1/4 turn    1/2 turn    3/4 turn    Back to start
  
If the motor starts in the "ON" region the signal is inverted:
───────────┐           ┌───────────┐           ┌─────────────
           │           │           │           │           
           │           │           │           │           
           └───────────┘           └───────────┘          
  Start      1/4 turn    1/2 turn    3/4 turn    Back to start
