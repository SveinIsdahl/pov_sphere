#ifndef SPI_H_
#define SPI_H_

void spi_init(void);
void spi_transmit(char cData);
void spi_transmit_32(uint32_t data);

#endif
