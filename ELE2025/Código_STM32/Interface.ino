//Human Interface code
#include "Main.h"

#define BuzzerPin PA8
#define LEDBlue PA0
#define LEDGreen PA1
#define LEDRed PB0

void setInterface(){
  pinMode(BuzzerPin, OUTPUT);
  pinMode(LEDBlue, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  pinMode(LEDRed, OUTPUT);
}

void buzzerOn(){
  digitalWrite(BuzzerPin, HIGH);
}

void buzzerOff(){
  digitalWrite(BuzzerPin, LOW);
}

void LED(char r, char g, char b){
  analogWrite(LEDRed,r*4);
  analogWrite(LEDGreen,g*4);
  analogWrite(LEDBlue,b*4);
}
