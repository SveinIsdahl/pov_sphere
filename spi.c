#include <avr/io.h>

#include "spi.h"


#define MOSI_bm          (1 << 4)
#define SCK_bm           (1 << 6)
#define SPI_PORT         PORTA
#define CS_bm            (1 << 6)
#define CS_PORT          PORTC

void spi_init() {
        SPI_PORT.DIRSET = (MOSI_bm) | (SCK_bm);

        CS_PORT.DIRSET = (CS_bm);

        CS_PORT.OUTSET = (CS_bm);
		
        SPI0.CTRLB |= (SPI_SSD_bm);
		
        SPI0.CTRLA |= (SPI_ENABLE_bm) | (SPI_MASTER_bm);//  | (SPI_CLK2X_bm)//Remove CLK2 if SPI is unstable. Goes from 2Mhz to 4 Mhz
}

void spi_transmit(char cData) {
        // Set CS low to select (active low)
        CS_PORT.OUTCLR = (CS_bm);

        SPI0.DATA = cData;

        // Wait until the the data is sent
        while (!(SPI0.INTFLAGS & (SPI_IF_bm))) {
			asm volatile("nop");
		}

        SPI0.DATA;

        // Deselect the CS
        CS_PORT.OUTSET = (CS_bm);
}
void spi_transmit_32(uint32_t data) {
	spi_transmit((uint8_t)data);
	spi_transmit((uint8_t)(data >> 8));
	spi_transmit((uint8_t)(data >> 16));
	spi_transmit((uint8_t)(data >> 24));
}
