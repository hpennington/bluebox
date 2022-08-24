#include "bluetooth.h"
#include "adc_unit.h"
#include "spi_unit.h"
#include "packetParser.h"
#include "sintab2.h"

Bluetooth bluetooth;
ADCUnit adc;
SPIUnit spi;

int lookup = 0;
bool isInitial = true;

void print_data(uint8_t data_length) {
    if (data_length == 0) return;
    
    // Accelerometer
    if (bluetooth.buffer[1] == 'A') {
        float x, y, z;
        x = parsefloat(bluetooth.buffer+2);
        y = parsefloat(bluetooth.buffer+6);
        z = parsefloat(bluetooth.buffer+10);
        Serial.print("Accel\t");
        Serial.print(x); Serial.print('\t');
        Serial.print(y); Serial.print('\t');
        Serial.print(z); Serial.println();
    }
}

bool finished_writing_SPI() {
    return (SPSR & _BV(SPIF));
}


void transfer(uint8_t data) {
    SPDR = data;
}


void setup_serial() {
  Serial.begin(115200);
}

void setup() {
    setup_serial();
    SPIUnit_setup(&spi);
    SPIUnit_start(&spi);
    
    Bluetooth_init(&bluetooth);

    ADCUnit_init(&adc);
    ADCUnit_start(&adc);
}

void loop() {
    uint8_t data_length = Bluetooth_read(&bluetooth);
    print_data(data_length);
}

ISR(ADC_vect) {
    if (!spi.is_transacting) {
        ADCUnit_update(&adc);
    }
    
    uint16_t out = (0 << 15) | (0 << 14) | (1 << 13) | (1 << 12) | adc.value;
    
    if (!spi.is_transacting && finished_writing_SPI() == true) {
        PORTD &= ~(1 << PD4);
        spi.is_transacting = true;
        isInitial = false;
        transfer((out & 0xff00) >> 8);
    } else if (isInitial == true) {
        PORTD &= ~(1 << PD4);
        spi.is_transacting = true;
        isInitial = false;
        transfer((out & 0xff00) >> 8);
    } else if (spi.is_transacting && finished_writing_SPI() == true && spi.is_last_byte == false) {
        transfer(out & 0xff);
        spi.is_last_byte = true;
    } else if (spi.is_transacting == true && spi.is_last_byte == true && finished_writing_SPI() == true) {
        PORTD |= (1 << PD4);
        spi.is_transacting = false;
        spi.is_last_byte = false;
    }
    
    ADCSRA |= B01000000;
}
