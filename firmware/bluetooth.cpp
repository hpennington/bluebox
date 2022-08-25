#include <string.h>
#include <Arduino.h>
#include "bluetooth.h"

uint8_t readPacket(Bluetooth *bluetooth, Adafruit_BLE *ble, uint16_t timeout);

void Bluetooth_init(Bluetooth *bluetooth) {
#ifdef SERIAL_PRINT_ON
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));
#endif
  bluetooth->bluefruitSS = new SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
  bluetooth->bluefruit = new Adafruit_BluefruitLE_UART(*bluetooth->bluefruitSS, BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

#ifdef SERIAL_PRINT_ON
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));
#endif
  if ( !bluetooth->bluefruit->begin(VERBOSE_MODE) )
  {
#ifdef SERIAL_PRINT_ON
    Serial.println("Error starting bluetooth serial transmission");
#endif
  }

#ifdef SERIAL_PRINT_ON
  Serial.println( F("OK!") );
#endif
  if ( FACTORYRESET_ENABLE )
  {
#ifdef SERIAL_PRINT_ON
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
#endif
    if ( ! bluetooth->bluefruit->factoryReset() ){
   #ifdef SERIAL_PRINT_ON
      Serial.println("Couldn't factory reset");
   #endif
    }
  }


  /* Disable command echo from Bluefruit */
  bluetooth->bluefruit->echo(false);
#ifdef SERIAL_PRINT_ON
  Serial.println("Requesting Bluefruit info:");
#endif
  /* Print Bluefruit information */
  bluetooth->bluefruit->info();
#ifdef SERIAL_PRINT_ON
  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();
#endif
  bluetooth->bluefruit->verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! bluetooth->bluefruit->isConnected()) {
      delay(500);
  }
#ifdef SERIAL_PRINT_ON
  Serial.println(F("******************************"));
#endif
  // LED Activity command is only supported from 0.6.6
  if ( bluetooth->bluefruit->isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    #ifdef SERIAL_PRINT_ON
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    #endif
    bluetooth->bluefruit->sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }
  #ifdef SERIAL_PRINT_ON
  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  #endif
  bluetooth->bluefruit->setMode(BLUEFRUIT_MODE_DATA);
#ifdef SERIAL_PRINT_ON
  Serial.println(F("******************************"));
#endif
}

uint8_t Bluetooth_read(struct Bluetooth *bluetooth) {
  uint8_t len = readPacket(bluetooth, bluetooth->bluefruit, BLE_READPACKET_TIMEOUT);
  return len;
}
