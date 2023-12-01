#include <avr/io.h>

void usart_init(uin16_t ubrr) {
  //Set baud
  UBRRH = (uint8_t)(ubrr >> 8);
  UBRRL = (uint8_t)ubrr;
  
  Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 1stop bit */
  UCSR0C = (3<<UCSZ00);
}

void usart_transmit(char data) {
  //Wait tfor empty transmit buffer
  while (!(UCSRA & (1<<UDRE)));
  
  UDR0 = data;
}

void usart_transmit_string(char *data) {
	while (*data != '\0') {
		usart_transmit(*data);
		data++;
	}
}
