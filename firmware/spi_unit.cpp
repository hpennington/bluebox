#include <SPI.h>
#include "spi_unit.h"

/**
 * Helper functions 
 */

void load_transfer(uint8_t data) {
    SPDR = data;
}

/*
 * Public interface functions
 */
void SPIUnit_setup(SPIUnit *spi) {
    pinMode(PIN_SS, OUTPUT);
    spi->is_transacting = false;
    spi->is_last_byte = false;
    spi->is_initial = true;
    spi->value = 0;
}

void SPIUnit_start(SPIUnit *spi) {
    SPI.begin();
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
}

void SPIUnit_transfer(SPIUnit *spi, uint16_t value) {
    spi->value = value;
}
