// Main Header

#ifndef MAIN_H
#define MAIN_H

struct _GPS{
  float Pos[3];
  int Time[3];
};

byte RxByte;
bool noice = false;
#endif