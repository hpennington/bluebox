#include <avr/io.h>
#include <Arduino.h>
#include "adc_unit.h"

void ADCUnit_init(ADCUnit *adc) {
    adc->value = 0;

    // setup ADC
    ADMUX = 0x60; // left adjust, adc0, internal vcc
    ADCSRA = 0xe0; // turn on adc, auto trigger
    
    // Set ADC clock prescalar to 128 // 16000000 / 128 / 13 (clock cylcles per conversion) = ~9600sps
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) & (1 << ADPS0);
    DIDR0 = 0x01; // turn off digital inputs for adc0
}

void ADCUnit_start(ADCUnit *adc) {
    ADCSRA |= B00001000; 
}

void ADCUnit_update(ADCUnit *adc) {
    uint8_t low = ADCL;
    uint8_t high = ADCH;
    uint16_t value = (high << 4) | (low >> 4);
    adc->value = value;
}
