// Main header
#ifndef MAIN_H
#define MAIN_H

#define SW0 PB5
#define SW1 PB4
#define SW2 PB3
#define SW3 PA15
#define SW4 PA2
#define SW5 PA3

#include <Wire.h>
TwoWire Wire2(PB11,PB10); // Wire 2 (PB11 = SDA, PB10 = SCL)

enum instruction : uint8_t {
  Invalid = 0x00, Ready, Send, DataMpu0Ac = 0x10, DataMpu0Gy, DataMpu0Mg, DataMpu1Ac=0x20,
  DataMpu1Gy, DataMpu1Mg, DataMpu2Ac=0x30, DataMpu2Gy, DataMpu2Mg, DataMpu3Ac=0x40,
  DataMpu3Gy, DataMpu3Mg,DataBMPAt = 0x50,DataBMPTp, DataHx711 = 0x60, DataBat = 0x70, 
  LEDR = 0x80, LEDG, LEDB, Switch0On = 0x90, Switch1On,Switch2On,Switch3On,Switch4On, 
  Switch5On, Switch0Off,Switch1Off,Switch2Off,Switch3Off,Switch4Off,Switch5Off, BuzzerOn = 0xA0, BuzzerOff};

volatile bool readSPI = false;
volatile instruction instruction_rx = Invalid;


#endif