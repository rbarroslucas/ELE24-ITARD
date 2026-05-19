//Hx711 reader using Queuetue Hx711 library by scott russel
#include "Main.h"
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = PA11;
const int LOADCELL_SCK_PIN = PA12;

HX711 scale;

void setHx711() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

long readHx711() {
  long reading = -1;
  if (scale.is_ready()) {
    reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
  return reading;
}