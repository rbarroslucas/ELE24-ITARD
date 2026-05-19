// I2C interface to RP pi
#include "Main.h"
#include <Wire.h>
#define SDA 17
#define SCL 22

void setI2C(){
  Wire.setPins(SDA , SCL);
  Wire.begin(0x68); // Initialize I2C (Slave Mode: address=0x55 )
  Wire.onReceive(I2C_RxHandler);
}

void I2C_RxHandler(int numBytes)
{
  while(Wire.available()) {  // Read Any Received Data
    RxByte = Wire.read();
  }
  noice = true;
}