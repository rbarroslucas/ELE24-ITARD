// BMP280 barometer basic testing software, please later use a lib as this sensor's calibration constants can
// There is one is currently tested at i2c2, perhaps test another one at i2c1 for two barometers?

#include "Main.h"
#define BMP 0x76      // BMP180 I2C address

void setBMP()
{
  Wire2.beginTransmission(BMP);      // Checks Chip Id
  Wire2.write(0xD0);                 // reads from register 0x09
  Wire2.endTransmission(false);
  Wire2.requestFrom(BMP,1,true);
  Serial.println(Wire2.read());
}