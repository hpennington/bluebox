#include <SPI.h>
#include "bluetooth.h"
#include "adc_unit.h"
#include "packetParser.h"
#include "sintab2.h"

Bluetooth bluetooth;
ADCUnit adc;

int lookup = 0;
volatile bool inTransaction = false;
volatile bool lastByte = false;
bool isInitial = true;
const int PIN_SS = 4;

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

void setup_serial() {
  Serial.begin(115200);
}

void setup_spi() {
    pinMode(PIN_SS, OUTPUT); 
}

void begin_spi() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
}

bool finished_writing_SPI() {
    return (SPSR & _BV(SPIF));
}

void transfer(uint8_t data) {
    SPDR = data;
}

void setup() {
    setup_serial();
    setup_spi();
    begin_spi();
    
    Bluetooth_init(&bluetooth);

    ADCUnit_init(&adc);
    ADCUnit_start(&adc);
}

void loop() {
    uint8_t data_length = Bluetooth_read(&bluetooth);
    print_data(data_length);
}

ISR(ADC_vect) {
    if (!inTransaction) {
        ADCUnit_update(&adc);
    }
    
    uint16_t out = (0 << 15) | (0 << 14) | (1 << 13) | (1 << 12) | adc.value;
    
    if (!inTransaction && finished_writing_SPI() == true) {
        PORTD &= ~(1 << PD4);
        inTransaction = true;
        isInitial = false;
        transfer((out & 0xff00) >> 8);
    } else if (isInitial == true) {
        PORTD &= ~(1 << PD4);
        inTransaction = true;
        isInitial = false;
        transfer((out & 0xff00) >> 8);
    } else if (inTransaction && finished_writing_SPI() == true && lastByte == false) {
        transfer(out & 0xff);
        lastByte = true;
    } else if (inTransaction == true && lastByte == true && finished_writing_SPI() == true) {
        PORTD |= (1 << PD4);
        inTransaction = false;
        lastByte = false;
    }

    ADCSRA |= B01000000;
}
