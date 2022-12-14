#include <avr/io.h>
#include <avr/interrupt.h>
#include "bluetooth.h"
#include "adc_unit.h"
#include "spi_unit.h"
#include "packetParser.h"
#include "effects/fuzz.h"
#include "effects/tremolo.h"
#include "print.h"

Bluetooth bluetooth;
ADCUnit adc;
SPIUnit spi;

bool finished_writing_SPI() {
    return (SPSR & _BV(SPIF));
}

void transfer(uint8_t data) {
    SPDR = data;
}

void setup_spi_timer() {
    TCCR1A = (1<<CS10) | (1<<CS12); //set the pre-scalar as 1024
    OCR1A = 32768;
    TCNT1 = 0;
    TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}

const int n_kernels = 2;

bool kernels_on[n_kernels] = {
    false,
    false
};

void setup() {
    setup_serial();
    
    SPIUnit_setup(&spi);
    SPIUnit_start(&spi);
    
    Bluetooth_init(&bluetooth);

    ADCUnit_init(&adc);
    ADCUnit_start(&adc);

    setup_spi_timer();

    setup_tremolo_timer();
    
    sei();
}

void loop() {
    uint8_t data_length = Bluetooth_read(&bluetooth);

    if (data_length > 0) {
        uint8_t command = bluetooth.buffer[2];
        switch(command) {
            case 1:
                kernels_on[0] = false;
                kernels_on[1] = false;
                break;
            case 2:
                kernels_on[0] = false;
                kernels_on[1] = true;
                break;
            case 3:
                kernels_on[0] = true;
                kernels_on[1] = false;
                break;
            case 4:
                kernels_on[0] = true;
                kernels_on[1] = true;
                break;
        }
    }
}

ISR(TIMER1_OVF_vect) {
    uint16_t out = (0 << 15) | (0 << 14) | (1 << 13) | (1 << 12) | adc.value; 

    if (!spi.is_transacting && finished_writing_SPI() == true) {
        PORTD &= ~(1 << PD4);
        spi.is_transacting = true;
        spi.is_initial = false;
        transfer((out & 0xff00) >> 8);
    } else if (spi.is_initial == true) {
        PORTD &= ~(1 << PD4);
        spi.is_transacting = true;
        spi.is_initial = false;
        transfer((out & 0xff00) >> 8);
    } else if (spi.is_transacting && finished_writing_SPI() == true && spi.is_last_byte == false) {
        transfer(out & 0xff);
        spi.is_last_byte = true;
    } else if (spi.is_transacting == true && spi.is_last_byte == true && finished_writing_SPI() == true) {
        PORTD |= (1 << PD4);
        spi.is_transacting = false;
        spi.is_last_byte = false;
    }
    
    TCNT1 = 0;
}

ISR(ADC_vect) {
    if (!spi.is_transacting) {
        ADCUnit_update(&adc);
        uint16_t value = adc.value;
        if (kernels_on[1] == true) {
            value = fuzz(value);
        }

        if (kernels_on[0] == true) {
            value = tremolo(value);
        }

        adc.value = value;
    }
    
    ADCSRA |= B01000000;
}
