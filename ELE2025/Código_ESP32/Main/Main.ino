// Main Execution Sequence

#include "Main.h"

_GPS GPS;

void setup() {
  Serial.begin(115200);
  setGPS();
  setI2C();
  setLoRa();
}

void loop(){
  if(noice){noice = false;
    Serial.println(RxByte);
  }

  GPS = readGPS();

  String packet = "Latitude: " + String(GPS.Pos[0], 6) + 
                  ", Longitude: " + String(GPS.Pos[1], 6) + 
                  ", Altitude: " + String(GPS.Pos[2], 2) + "\n" + 
                  String(GPS.Time[2]) + ":" + String(GPS.Time[1]) + ":" + String(GPS.Time[0]);

  Serial.println(packet);

  sendLoRa(packet);

  delay(1000);
}
