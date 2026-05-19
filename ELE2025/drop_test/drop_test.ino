#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

#define CS 4
#define SKIB 3
#define BUZ 6
#define LED 8
#define BUFFER_SIZE 10
#define PRESSURE_REF 1023.00

Adafruit_BMP280 bar;

File file;
float altBuffer[BUFFER_SIZE] = {0};
float velBuffer[BUFFER_SIZE] = {0};
float altAvg = 0, velAvg = 0, altGround = 0, altMax = 0;
const float minAltitudeLaunch = 10.0;
const float minAltitudeDeploy = 2.0;
unsigned long previousMillis;
int index = 0;
bool inFlight = false, skibActivated = false;

void calibrate();
void ground();
void flight();

void setup() {
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  pinMode(SKIB, OUTPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(LED, OUTPUT);

  while (!bar.begin()) {
    Serial.println("Barometer not found!");
    delay(2000);
  }
  tone(BUZ, 400, 1000);
  delay(1250);
  Serial.println("Barometer initialized!");

  while (!SD.begin(CS)) {
    Serial.println("SD initialization failed!");
    delay(2000);
  }
  Serial.println("SD initialized!");
  tone(BUZ, 400, 200);
  delay(250);
  tone(BUZ, 400, 200);
  delay(250);

  file = SD.open("dados_ipev.txt", FILE_WRITE);
  if (file) {
    file.println("Clock,Altitude,Pressure,Temp,AltAvg,VelAvg,AltMax,Event");
    file.close();
  }
  digitalWrite(LED, HIGH);
  tone(BUZ, 400, 200);
  delay(250);
  tone(BUZ, 400, 200);
  delay(250);

  calibrate();
}

void loop() {
  if (!inFlight) {
    ground();
  } else {
    flight();
  }
}

void calibrate() {
  altAvg = 0;
  for (index = 0; index < BUFFER_SIZE; index++) {
    altBuffer[index] = bar.readAltitude(PRESSURE_REF);
    altAvg += altBuffer[index] / BUFFER_SIZE;
    delay(50);
  }
  altGround = altAvg;
  altMax = altAvg;
  index = 0;
  previousMillis = millis();
}

void ground() {
  updateVelocity();

  if (altBuffer[index] - altGround < minAltitudeLaunch) {
    inFlight = true;
  }

  logData("GROUND");
  delay(75);
  digitalWrite(LED, LOW);
}

void flight() {
  updateVelocity();

  if (!skibActivated && ) {
    digitalWrite(SKIB, HIGH);
    skibActivated = true;
    logEvent("SKIB ACTIVATED");
  } else {
    digitalWrite(SKIB, LOW);
  }

  if (altAvg - altGround < minAltitudeLaunch) {
    digitalWrite(SKIB, LOW);
    altMax = altAvg;
    inFlight = false;
    skibActivated = false;
  }

  logData("FLIGHT");
  delay(75);
}

void updateVelocity() {
  velAvg -= velBuffer[index] / BUFFER_SIZE;
  float currentAlt = bar.readAltitude(PRESSURE_REF);
  unsigned long currentMillis = millis();
  velBuffer[index] = (currentAlt - altAvg) * 1000.0 / (currentMillis - previousMillis);
  velAvg += velBuffer[index] / BUFFER_SIZE;

  previousMillis = currentMillis;

  altAvg -= altBuffer[index] / BUFFER_SIZE;
  altBuffer[index] = currentAlt;
  altAvg += altBuffer[index] / BUFFER_SIZE;

  if (altAvg > altMax) altMax = altAvg;

  index++;
  if (index >= BUFFER_SIZE) index = 0;
}

void logData(const char *state) {
  file = SD.open("dados_ipev.txt", FILE_WRITE);
  if (file) {
    file.print((float)millis() / 1000.0);
    file.print(',');
    file.print(bar.readAltitude(PRESSURE_REF));
    file.print(',');
    file.print(bar.readPressure());
    file.print(',');
    file.print(bar.readTemperature());
    file.print(',');
    file.print(altAvg);
    file.print(',');
    file.print(velAvg);
    file.print(',');
    file.print(altMax);
    file.print(',');
    file.println(state);
    file.close();
  }

  Serial.print((float)millis() / 1000.0);
  Serial.print('\t');
  Serial.print(bar.readAltitude(PRESSURE_REF));
  Serial.print('\t');
  Serial.print(bar.readPressure());
  Serial.print('\t');
  Serial.print(bar.readTemperature());
  Serial.print('\t');
  Serial.print(altAvg);
  Serial.print('\t');
  Serial.print(velAvg);
  Serial.print('\t');
  Serial.print(altMax);
  Serial.print('\t');
  Serial.println(state);
}

void logEvent(const char *event) {
  file = SD.open("dados_ipev.txt", FILE_WRITE);
  if (file) {
    file.println(event);
    file.close();
  }
  Serial.println(event);
}
