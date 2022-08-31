#include <string.h>
#include <Arduino.h>
#include "print.h"
#include "bluetooth.h"

uint8_t readPacket(Bluetooth *bluetooth, Adafruit_BLE *ble, uint16_t timeout);

void Bluetooth_init(Bluetooth *bluetooth) {
    print_serial("\nInitializing software serial and bluefruit library");
    bluetooth->bluefruitSS = new SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
    bluetooth->bluefruit = new Adafruit_BluefruitLE_UART(*bluetooth->bluefruitSS, BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

    if (!bluetooth->bluefruit->begin(VERBOSE_MODE)) {
        print_serial("Error starting bluetooth serial transmission");
    }

    if (FACTORYRESET_ENABLE) {
        print_serial("Performing a factory reset: ");
        if (!bluetooth->bluefruit->factoryReset()){
            print_serial("Couldn't factory reset");
        }
    }

    bluetooth->bluefruit->echo(false);
    print_serial("Requesting Bluefruit info:");
    
    bluetooth->bluefruit->info();
    bluetooth->bluefruit->verbose(false);  // debug info is a little annoying after this point!

    print_serial("waiting for connection....");

    while (!bluetooth->bluefruit->isConnected()) {
        delay(500);
    }

    print_serial("******************************");

    // LED Activity command is only supported from 0.6.6
    if (bluetooth->bluefruit->isVersionAtLeast(MINIMUM_FIRMWARE_VERSION)) {
        bluetooth->bluefruit->sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    }
    
    // Set Bluefruit to DATA mode
    print_serial("Switching to DATA mode!");
    bluetooth->bluefruit->setMode(BLUEFRUIT_MODE_DATA);
    print_serial("******************************");
}

uint8_t Bluetooth_read(struct Bluetooth *bluetooth) {
    uint8_t len = readPacket(bluetooth, bluetooth->bluefruit, BLE_READPACKET_TIMEOUT);
    return len;
}
