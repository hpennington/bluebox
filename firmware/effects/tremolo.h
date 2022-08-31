#include "../sintab2.h"

volatile uint16_t tick = 0;

inline uint16_t tremolo(uint16_t value) {
    return (float)value * ((float)sintab2[tick] / 4096.0);
}

void setup_tremolo_timer() {
    TCCR2A = (0<<CS02) | (1<<CS01) | (1<<CS00); //set the pre-scalar as 1024
    OCR2A = 50;
    TCNT2 = 0;
    TIMSK2 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}

ISR(TIMER2_OVF_vect) {
    tick += 1;

    if (tick > 300) {
        tick = 0;
    }
}

