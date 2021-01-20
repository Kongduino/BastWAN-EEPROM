#include <Wire.h>

#include "SparkFun_External_EEPROM.h"
// Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM
/*
You need to define the buffer lengths in SparkFun_External_EEPROM.h
Around line 56:
#elif defined(_VARIANT_ELECTRONICCATS_BASTWAN_)
#define I2C_BUFFER_LENGTH_RX SERIAL_BUFFER_SIZE
#define I2C_BUFFER_LENGTH_TX SERIAL_BUFFER_SIZE
*/
ExternalEEPROM myMem;
unsigned char buf[256];

void setup() {
  SerialUSB.begin(115200);
  SerialUSB.flush();
  delay(3000);
  Serial.println("\nQwiic EEPROM example");
  Serial.println("Buffer size: " + String(SERIAL_BUFFER_SIZE));
  Wire.begin(SDA, SCL);
  Wire.setClock(100000);
  // bool begin(uint8_t deviceAddress = 0b1010000, TwoWire &wirePort = Wire);
  // deviceAddress = 0x50
  // flip switches on the breakout board to change it to between 0x51 and 0x57.
  if (myMem.begin() == false) {
    Serial.println("No memory detected. Freezing.");
    while (1)
      ;
  }
  uint32_t myLen = myMem.length(), index = 0;
  Serial.println("Memory detected!");
  Serial.print("Mem size in bytes: ");
  Serial.println(myLen);
  myLen = 2048;
  // Just a few blocks
  while (myLen > 0) {
    for (uint16_t i = 0; i < 256; i++) buf[i] = myMem.read(index++);
    hexDump(256);
    myLen -= 256;
  }
}

void loop() {
}

void hexDump(uint16_t len) {
  String s = "|", t = "| |";
  Serial.println(F("  |.0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .a .b .c .d .e .f |"));
  Serial.println(F("  +------------------------------------------------+ +----------------+"));
  for (uint16_t i = 0; i < len; i += 16) {
    for (uint8_t j = 0; j < 16; j++) {
      if (i + j >= len) {
        s = s + "   "; t = t + " ";
      } else {
        char c = buf[i + j];
        if (c < 16) s = s + "0";
        s = s + String(c, HEX) + " ";
        if (c < 32 || c > 127) t = t + ".";
        else t = t + (String(c));
      }
    }
    uint8_t index = i / 16;
    Serial.print(index, HEX); Serial.write('.');
    Serial.println(s + t + "|");
    s = "|"; t = "| |";
  }
  Serial.println(F("  +------------------------------------------------+ +----------------+"));
}
