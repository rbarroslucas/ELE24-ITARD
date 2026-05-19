// Main execution code

#include "Main.h"

void setup() {
  /* Serial1 to display data */
  //setSwitches();
  Serial.begin(9600);
  Serial.println("Olá do STM32!");
  while(!Serial) {}
  //Serial.println(setMPU());
  //setBMP();
  //setHx711();
  //setBat();
  //setInterface();
  //LED(0,0,0);
  //setSPI();
}

void loop() {
  //if (readSPI) {
  //  Serial.println(instruction_rx);
  //  readSPI = false;
  //}
  Serial.println("Teste");
  delay(1000);
  /* Check if data read */
  //buzzerOn();
  //turnSwitch(1,HIGH);
  //LED(255,0,0);
  //readMPU(2);
  //Serial.println(readHx711());
  //Serial.println(readBat());
}