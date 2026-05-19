//Simple battery voltage reader implementation

#include "Main.h"

void setBat(){
 pinMode(PB1, INPUT); // It is already default input but... why not?
}

float readBat(){
  const float Res1 = 200; // Vbat to P
  const float Res2 = 200; // P to GND
  const float ADCScale = 3.3/4096; // Vref / Resolution

  return  ADCScale*((float) adc_read_value(PB_1, 12))*((Res1+Res2)/Res2);
}