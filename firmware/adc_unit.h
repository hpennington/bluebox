#include <stdint.h>

typedef struct ADCUnit {
    uint16_t value;
} ADCUnit;

void ADCUnit_init(ADCUnit *adc);
void ADCUnit_start(ADCUnit *adc);
void ADCUnit_update(ADCUnit *adc);
