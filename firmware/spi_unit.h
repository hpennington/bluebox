#define PIN_SS 4

typedef struct SPIUnit {
    volatile uint16_t value;
    volatile bool is_transacting;
    volatile bool is_last_byte;
    bool is_initial;
} SPIUnit;

void SPIUnit_setup(SPIUnit *spi);
void SPIUnit_start(SPIUnit *spi);
void SPIUnit_transfer(SPIUnit *spi, uint16_t value);
