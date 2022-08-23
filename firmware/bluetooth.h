#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLE_READPACKET_TIMEOUT         500   // Timeout in ms waiting to read a response
#define BLUEFRUIT_SWUART_RXD_PIN       9    // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN       10   // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN         6   // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN         8   // Optional, set to -1 if unused
#define BLUEFRUIT_UART_MODE_PIN        7    // Set to -1 if unused
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
#define READ_BUFSIZE                    (20)

typedef struct Bluetooth {
  SoftwareSerial *bluefruitSS;
  Adafruit_BluefruitLE_UART *bluefruit;
  uint8_t buffer[READ_BUFSIZE+1];
} Bluetooth;

void Bluetooth_init(Bluetooth *bluetooth);
uint8_t Bluetooth_read(Bluetooth *bluetooth);
