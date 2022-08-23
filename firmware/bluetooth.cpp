#include <string.h>
#include <Arduino.h>
#include "bluetooth.h"

uint8_t readPacket(Bluetooth *bluetooth, Adafruit_BLE *ble, uint16_t timeout);

void Bluetooth_init(Bluetooth *bluetooth) {
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));
  bluetooth->bluefruitSS = new SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
  bluetooth->bluefruit = new Adafruit_BluefruitLE_UART(*bluetooth->bluefruitSS, BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
  
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !bluetooth->bluefruit->begin(VERBOSE_MODE) )
  {
    Serial.println("Error starting bluetooth serial transmission");
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! bluetooth->bluefruit->factoryReset() ){
      Serial.println("Couldn't factory reset");
    }
  }


  /* Disable command echo from Bluefruit */
  bluetooth->bluefruit->echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  bluetooth->bluefruit->info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  bluetooth->bluefruit->verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! bluetooth->bluefruit->isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( bluetooth->bluefruit->isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    bluetooth->bluefruit->sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  bluetooth->bluefruit->setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
}

uint8_t Bluetooth_read(struct Bluetooth *bluetooth) {
  uint8_t len = readPacket(bluetooth, bluetooth->bluefruit, BLE_READPACKET_TIMEOUT);
  return len;
}
