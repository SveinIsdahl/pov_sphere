#define F_CPU 3333333UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/builtins.h>
#include <stdio.h>
#include <string.h>


#include "spi.h"
#include "img_example.h"
//Display config
#define VERT_SIZE 32 //Should always be the amount of leds to ensure all leds are cleared


//Pins
//Tacho = 
#define TACHO_PIN 7
#define TACHO_PORT PORTC

#define BAUD_9600         4 * F_CPU / 9600
#define TX_PORT           PORTB
#define TX_PIN            0

char uart_str[8];

void usart_init(unsigned long baud) {
	TX_PORT.DIRSET = (1 << TX_PIN);

	// Set baud rate we've specified. 
	USART3.BAUDL = (uint8_t)baud;
	USART3.BAUDH = (uint8_t)(baud >> 8);

	// Enable RX and TX
	USART3.CTRLB = (1 << USART_RXEN_bp) | (1 << USART_TXEN_bp);
}

void usart_transmit(char data) {
	// Sends a character over UART

	// First we should check that there isn't already data being sent.
	// If there is, we should wait for it to finish first
	while (!(USART3.STATUS & (1 << USART_DREIF_bp))) {}

	// Put our new data into TX data register
	USART3.TXDATAL = data;
}

void usart_transmit_string(char *data) {
	// Loop until the end of the string
	while (*data != '\0') {
		usart_transmit(*data);
		data++;
	}
}
//Globals
//Clock cycles per 3/4s rotations
//uint16_t clock_cycles_prev = 0;
//uint16_t clock_cycles = 0;

static void send_pixel_array(Pixel pixels[], int size) {
	spi_transmit_32(0x0); //Start transmission
	for(int i = 0; i < size; i++) {
		spi_transmit(0xE0 | pixels[i].brightness);
		spi_transmit(pixels[i].blue);      //BLUE
		spi_transmit(pixels[i].green);     //GREEN
		spi_transmit(pixels[i].red);       //RED
	}
	spi_transmit_32(0xffffffff); //Stop
}
static void clear_pixel_array(int size) {
	spi_transmit_32(0x0); //Start transmission
	for(int i = 0; i < size; i++) {
		spi_transmit(0xE0);
		spi_transmit(0);      
		spi_transmit(0);     
		spi_transmit(0);
	}
	spi_transmit_32(0xffffffff); //Stop
}
Pixel test[16] = {{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0, 0, 0x0, 0xff, },{0, 0, 0x0, 0xff, },{0, 0, 0x0, 0xff, },{0, 0, 0x0, 0xff, },{0, 0, 0x0, 0xff, },{0, 0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff, },{0x0, 0x0, 0x0, 0xff}};

volatile uint8_t print = 0;
volatile int print_val_timer = 0;

int main(void) {
	usart_init(BAUD_9600);
	spi_init();
	sei();

	//16 bit timer
	TCA0.SINGLE.PER = 0xFFFF;//45;//0x0001;
	TCA0.SINGLE.CTRLA |= (TCA_SINGLE_CLKSEL_DIV256_gc) | (TCA_SINGLE_ENABLE_bm);
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0EN_bm;
	TCA0.SINGLE.CMP0 = 45;
	
	//Tachometer init 
	TACHO_PORT.DIRCLR = (1 << TACHO_PIN); //input
	TACHO_PORT.PIN7CTRL |= PORT_PULLUPEN_bm;
	TACHO_PORT.PIN7CTRL |= PORT_ISC_RISING_gc; //Generate interrupt on both edges
	sei(); //Enable interrupts globally

	while (1) {
		sprintf(uart_str, "%d\n", (TCA0.SINGLE.CNT));
		usart_transmit_string(uart_str);
	}
}
int i = 0;
/*
ISR(TCA0_OVF_vect) { //Called every 1/32th of a rotation
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; //reset interrupt
	i++;
	//Show single strip
	if(i == 32) {
		i = 0;
	}
	if(i < IMG_W) {
		//send_pixel_array(image[i], IMG_W);
	}
	if(i >= IMG_W) {
		//send_pixel_array(test, IMG_W);
	}
}
*/
ISR(TCA0_CMP0_vect) {
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; //reset interrupt
	i++;
	//Show single strip
	if(i == 32) {
		i = 0;
	}
	//clear_pixel_array(32);
	if(i < IMG_W) {
		send_pixel_array(image[i], IMG_W);
	}
	if(i >= IMG_W) {
		//send_pixel_array(test, IMG_W);
	}

}

ISR(PORTC_PORT_vect) { //Interrupt is called every 3/4s rotations
		TACHO_PORT.INTFLAGS = (1 << TACHO_PIN); //Clear interrupt
		TCA0.SINGLE.CMP0 = (int) (TCA0.SINGLE.CNT * (4/3) * 1/32);
		print_val_timer = TCA0.SINGLE.CNT;
		TCA0.SINGLE.CNT = 0; //Reset timer
		usart_transmit_string("test");
}
