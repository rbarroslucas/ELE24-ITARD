#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

#define CS 4
#define SKIB 3
#define BUZ 8
#define M 10
#define H 1023.00

Adafruit_BMP280 bar;

File MedM;
float h[M] = {0}, hm = 0, hsolo, hmax, v[M] = {0}, vm = 0;
unsigned long time;
int i = 0;
bool voo = 0;

void calib ( );
void solo ( );
void gogogo ( );

void setup ( ) {
  Serial.begin (9600);
  pinMode (CS, OUTPUT);
  pinMode (SKIB, OUTPUT);
  pinMode (BUZ, OUTPUT);
  beginbar: if (!bar.begin ( )) {
    Serial.println("Barometro não encontrado. Verifique as conexoes!");
    delay (3000);
    goto beginbar;
  } else {
    Serial.println("Barometro initialized!");
    tone (BUZ, 400, 1000);
    delay (1250);
  }
  beginsd: if (!SD.begin (CS)) {
    Serial.println("SD card initialization failed!");
    delay (3000);
    goto beginsd;
  } else {
    Serial.println("SD initialized!");
    tone (BUZ, 400, 200);
    delay (250);
    tone (BUZ, 400, 200);
    delay (250);
  }
  MedM = SD.open ("MedM.txt", FILE_WRITE);
  if (MedM) {
    MedM.println ("Clock, altitude, preassure, temperature, hm, vm, hmax:");
    MedM.close ( );
  }
  while (Serial.read ( ) != '1') {
    hm = 0;
    vm = 0;
    calib ( );
  }
}

void loop ( ) {
  if (voo == 0) {
    solo ( );
  }
  if (voo == 1) {
    gogogo ( );
  }
}

void calib ( ) {
  time = millis ( );
  for (i = 0; i < M; i++) {
    h[i] = bar.readAltitude (H);
    hm += h[i]/M;
  }
  for (i = 0; i < M; i++) {
    if (i == 0)
      v[i] = (bar.readAltitude (H) - hm)*M*1000/(millis ( ) - time);
    else
      v[i] = (bar.readAltitude (H) - hm)*1000/(millis ( ) - time);
    vm += v[i]/M;
    time = millis ( );
    delay (75);
    hm -= h[i]/M;
    h[i] = bar.readAltitude (H);
    hm += h[i]/M;
  }
  hsolo = hm;
  hmax = hm;
  i = 0;
}

void solo ( ) {
  vm -= v[i]/M;
  v[i] = (bar.readAltitude (H) - hm)*1000/(millis ( ) - time);
  vm += v[i]/M;
  if (h[i] - hsolo < 10 && fabs (vm) >= 2)
    voo = 1;
  MedM = SD.open ("MedM.txt", FILE_WRITE);
  if (MedM) {
    MedM.print ((float) millis ( )/1000.0);
    MedM.print ('\t');
    MedM.print (bar.readAltitude (H));
    MedM.print ('\t');
    MedM.print (bar.readPressure ( ));
    MedM.print ('\t');
    MedM.print (bar.readTemperature ( ));
    MedM.print ('\t');
    MedM.print (hm);
    MedM.print ('\t');
    MedM.println (vm);
    MedM.close ( );
  }
  Serial.print ((float) millis ( )/1000.0);
  Serial.print ('\t');
  Serial.print (bar.readAltitude (H));
  Serial.print ('\t');
  Serial.print (bar.readPressure ( ));
  Serial.print ('\t');
  Serial.print (bar.readTemperature ( ));
  Serial.print ('\t');
  Serial.print (hm);
  Serial.print ('\t');
  Serial.println (vm);
  time = millis ( );
  delay (75);
  hm -= h[i]/M;
  h[i] = bar.readAltitude (H);
  hm += h[i]/M;
  if (hm > hmax)
    hmax = hm;
  i++;
  if (i >= M)
    i = 0;
}

void gogogo ( ) {
  vm -= v[i]/M;
  v[i] = (bar.readAltitude (H) - hm)*1000/(millis ( ) - time);
  vm += v[i]/M;
  time = millis ( );
  while (millis ( ) - time < 75) {
    MedM = SD.open ("MedM.txt", FILE_WRITE);
    if (MedM) {
      MedM.print ((float) millis ( )/1000.0);
      MedM.print ('\t');
      MedM.print (bar.readAltitude (H));
      MedM.print ('\t');
      MedM.print (bar.readPressure ( ));
      MedM.print ('\t');
      MedM.print (bar.readTemperature ( ));
      MedM.print ('\t');
      MedM.print (hm);
      MedM.print ('\t');
      MedM.print (vm);
      MedM.print ('\t');
      MedM.println (hmax);
      MedM.close ( );
    }
    Serial.print ((float) millis ( )/1000.0);
    Serial.print ('\t');
    Serial.print (bar.readAltitude (H));
    Serial.print ('\t');
    Serial.print (bar.readPressure ( ));
    Serial.print ('\t');
    Serial.print (bar.readTemperature ( ));
    Serial.print ('\t');
    Serial.print (hm);
    Serial.print ('\t');
    Serial.print (vm);
    Serial.print ('\t');
    Serial.println (hmax);
  }
  hm -= h[i]/M;
  h[i] = bar.readAltitude (H);
  hm += h[i]/M;
  if (hmax > hm && hmax - hm < 5 && hmax - hsolo > 5 && fabs (vm) < 5) {
    digitalWrite (SKIB, HIGH);
    MedM = SD.open ("MedM.txt", FILE_WRITE);
    if (MedM) {
      MedM.println ("kabaklaushhshshhh");
      MedM.close ( );
    }
    Serial.println ("kabaklaushhshshhh");
  } else
    digitalWrite (SKIB, LOW);
  if (hm - hsolo < 10 && fabs (vm) < 2) {
    digitalWrite (SKIB, LOW);
    hmax = hm;
    voo = 0;
  }
  if (hm > hmax)
    hmax = hm;
  i++;
  if (i >= M)
    i = 0;
}