#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_) && not defined(__SAMD51__)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "bluetooth.h"
#include "packetParser.h"

#define PACKET_ACC_LEN                  (15)
#define PACKET_GYRO_LEN                 (15)
#define PACKET_MAG_LEN                  (15)
#define PACKET_QUAT_LEN                 (19)
#define PACKET_BUTTON_LEN               (5)
#define PACKET_COLOR_LEN                (6)
#define PACKET_LOCATION_LEN             (15)

#define SERIAL_PRINT_ON;

/**************************************************************************/
/*!
    @brief  Casts the four bytes at the specified address to a float
*/
/**************************************************************************/
float parsefloat(uint8_t *buffer) 
{
  uint8_t f;
  memcpy(&f, buffer, 1);
  return f;
}

/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters
    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void printHex(const uint8_t * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    Serial.print(F("0x"));
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
    {
      Serial.print(F("0"));
      Serial.print(data[szPos] & 0xf, HEX);
    }
    else
    {
      Serial.print(data[szPos] & 0xff, HEX);
    }
    // Add a trailing space if appropriate
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      Serial.print(F(" "));
    }
  }
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  Waits for incoming data and parses it
*/
/**************************************************************************/
uint8_t readPacket(Bluetooth *bluetooth, Adafruit_BLE *ble, uint16_t timeout) 
{
  
  uint16_t origtimeout = timeout, replyidx = 0;

  memset(bluetooth->buffer, 0, READ_BUFSIZE);

  while (timeout--) {
    if (replyidx >= 20) break;
    if ((bluetooth->buffer[1] == 'A') && (replyidx > 1))
      break;
    if ((bluetooth->buffer[1] == 'G') && (replyidx == PACKET_GYRO_LEN))
      break;
    if ((bluetooth->buffer[1] == 'M') && (replyidx == PACKET_MAG_LEN))
      break;
    if ((bluetooth->buffer[1] == 'Q') && (replyidx == PACKET_QUAT_LEN))
      break;
    if ((bluetooth->buffer[1] == 'B') && (replyidx == PACKET_BUTTON_LEN))
      break;
    if ((bluetooth->buffer[1] == 'C') && (replyidx == PACKET_COLOR_LEN))
      break;
    if ((bluetooth->buffer[1] == 'L') && (replyidx == PACKET_LOCATION_LEN))
      break;
      
    while (ble->available()) {
      char c =  ble->read();
      if (c == '!') {
        replyidx = 0;
      }
      bluetooth->buffer[replyidx] = c;
      replyidx++;
      timeout = origtimeout;
    }
    
    if (timeout == 0) break;
    delay(1);
  }
 
  bluetooth->buffer[replyidx] = 0;  // null term

  if (!replyidx) { // no data or timeout 
//    Serial.println("No data or timeout");
    return 0;
  }
  if (bluetooth->buffer[0] != '!') { // doesn't start with '!' packet beginning
//    Serial.println("doesnt start with !");
    return 0;
  }
  
  // check checksum!
  uint8_t xsum = 0;
  uint8_t checksum = bluetooth->buffer[replyidx-1];
  
  for (uint8_t i=0; i<replyidx-1; i++) {
    xsum += bluetooth->buffer[i];
  }
  xsum = ~xsum;

  // Throw an error message if the checksum's don't match
//  if (xsum != checksum)
//  {
//#ifdef SERIAL_PRINT_ON
//    Serial.print("Checksum mismatch in packet : ");
//    printHex(bluetooth->buffer, replyidx+1);
//#endif
//    return 0;
//  }
  
  // checksum passed!
  return replyidx;
}
