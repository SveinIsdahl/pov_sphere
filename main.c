/* REV 2 software
This is the start of a rewrite of the previous version for an ATmega 88A because the ATmega4809 perviously used was a devkit. 
Downgrading may seem weird, but the main limitation is the speed of the SPI bus over the slipring anyways. 
Tangent: To fix this on a larger design a more robust differential protocol (USB, ethernet, CAN) could be used or a wireless transfer of data, both require a chip translating to SPI on the spinning part of the assembly.
The main problem which needs to be fixed for rev 2 is to implement a PWM speed controller for the motor on the AVR.
*/

/* Modules needed
HW level drivers:
	UART for communication with PC
	SPI for LEDs
 	Timer/counter
		PWM
  		Speed sensing
speed control (possibly a PI controller to ensure stable speed)
Proper display driver abstraction

*/
