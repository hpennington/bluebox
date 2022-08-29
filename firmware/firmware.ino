#include <avr/io.h>
#include <avr/interrupt.h>
#include "bluetooth.h"
#include "adc_unit.h"
#include "spi_unit.h"
#include "packetParser.h"
#include "sintab2.h"

#define SERIAL_PRINT_ON true

Bluetooth bluetooth;
ADCUnit adc;
SPIUnit spi;

void print_data(uint8_t data_length) {
    if (data_length == 0) return;
    
    // Accelerometer
    if (bluetooth.buffer[1] == 'A') {
        uint8_t x = parsefloat(bluetooth.buffer+2);
        Serial.println(x);
    }
}

bool finished_writing_SPI() {
    return (SPSR & _BV(SPIF));
}

void transfer(uint8_t data) {
    SPDR = data;
}

void setup_serial() {
#ifdef SERIAL_PRINT_ON
    Serial.begin(115200);
#endif
}

void setup_spi_timer() {
    TCCR1A = (1<<CS10) | (1<<CS12); //set the pre-scalar as 1024
    OCR1A = 32768;
    TCNT1 = 0;
    TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}

volatile uint16_t min_value = 4095;
volatile uint16_t max_value = 0;
volatile uint16_t t = 0;

const int n_kernels = 2;

int kernels[n_kernels] = {
    1,
    0
};

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
    
    sei();
}

void loop() {
    uint8_t data_length = Bluetooth_read(&bluetooth);
#ifdef SERIAL_PRINT_ON
    print_data(data_length);
#endif

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

uint16_t clip(uint16_t value, uint16_t min_value, uint16_t max_value) {
    if (value >= min_value && value <= max_value) {
      return value;
    } else if (value < min_value) {
      return min_value;
    } else {
      return max_value;
    }
}

uint16_t fuzz(uint16_t value) {
    if (value > max_value) {
      max_value = value;
    } else if (value < min_value) {
      min_value = value;
    }

    uint16_t half_point = min_value + ((max_value - min_value) / 2);
    
    value = clip(adc.value, half_point - 200, half_point + 200); 

    if (t == 6500) {
        min_value = half_point;
        max_value = half_point;
        t = 0;
    }

    t += 1;

    return value;
}

uint16_t tremolo(uint16_t value) {
    return value;
}

uint16_t render(uint16_t value) {
    for (int i = 0; i < n_kernels; i += 1) {
        int kernel_id = kernels[i];

        switch(kernel_id) {
            case 0:
                if (kernels_on[i] == true) {
                    value = fuzz(value);  
                }
                break;
            case 1:
                if (kernels_on[i] == true) {
                    value = tremolo(value);  
                }
                break;
        }
    }

    return value;
}

ISR(TIMER1_OVF_vect) {
    uint16_t out = (0 << 15) | (0 << 14) | (1 << 13) | (1 << 12) | render(adc.value); 

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
    }
    
    ADCSRA |= B01000000;
}
