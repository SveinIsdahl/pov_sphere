
uint8_t spi_init() {
  cli();
  //Init GPIO
  
  
  //Master mode and enable.
  //Default clock phase/polarity and data order, SCK==F_CPU/4, 
  SPCR = (1 << MSTR) | (1 << SPE);
  sei();
}

uint8_t spi_transmit(char data) {
  //Check status register instead of having a blocking while loop
  if(!(SPSR & (1<<SPIF))) {
    return 1;
  }
  SPDR = data;
  return 0;
}

uint8_t spi_transmit_32(uint32_t data) {
	spi_transmit((char)data);
	spi_transmit((char)(data >> 8));
	spi_transmit((char)(data >> 16));
	spi_transmit((char)(data >> 24));
}
