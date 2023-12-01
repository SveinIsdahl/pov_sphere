#include <avr/io.h>

void usart_init(uin16_t ubrr) {
  //Set baud
  UBRRH = (uint8_t)(ubrr >> 8);
  UBRRL = (uint8_t)ubrr;
  
  //Enable receiver and transmitter
  UCSRB = (1<<RXEN)|(1<<TXEN);
  
  //Set frame format: 8 data bits, 1 stop bit
  UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void usart_transmit(char data) {
  //Wait tfor empty transmit buffer
  while (!(UCSRA & (1<<UDRE)));
  
  UDR = data;
}

void usart_transmit_string(char *data) {
	while (*data != '\0') {
		usart_transmit(*data);
		data++;
	}
}
