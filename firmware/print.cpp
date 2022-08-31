#include <Arduino.h>
#include "print.h"

void setup_serial() {
#ifdef PRINT_SERIAL_ON
    Serial.begin(115200);
#endif
}

void print_serial(char *str) {
#ifdef PRINT_SERIAL_ON
    Serial.println(str);
#endif
}

void print_serial(int value) {
#ifdef PRINT_SERIAL_ON
    Serial.println(value);
#endif
}
