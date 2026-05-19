#include <TinyGPSPlus.h>    
#include "Main.h"

#define RXD1 36             
#define TXD1 23              
#define GPS_Serial_Baud 9600 

TinyGPSPlus gps;   

void setGPS(){
  Serial1.begin(GPS_Serial_Baud, SERIAL_8N1, RXD1, TXD1);
}

_GPS readGPS(){
  _GPS GPS;
  while(Serial1.available() > 0) {                    
    if (gps.encode(Serial1.read())){                         
      if (gps.location.isValid() && gps.altitude.isValid()){                                  
        GPS.Pos[0] = gps.location.lat();               
        GPS.Pos[1] = gps.location.lng();               
        GPS.Pos[2] = gps.altitude.meters();               
      }                          
      if (gps.time.isValid()){                                  
        GPS.Time[0] = gps.time.second();                    
        GPS.Time[1] = gps.time.minute();               
        GPS.Time[2] = gps.time.hour();                
      }    
    }
  }
  return GPS;
}