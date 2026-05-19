#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

#define CS 4
#define SKIB 3
#define LED 8
#define BUZZER 10
#define M 10
#define H 1023.00
#define Hmin 3.0
#define Hmin_rec 10.0
#define Vmin 5.0

Adafruit_BMP280 bar;

File file;
float h[M] = {0}, v[M] = {0};
float hm = 0, vm = 0;
float hsolo, hmax;
unsigned long time;
unsigned long lastbeep = 0;
int i = 0;
bool voo = 0;

void calib();
void solo();
void flight();

void setup() {
  Serial.begin(9600);

  pinMode(CS, OUTPUT);
  pinMode(SKIB, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(SKIB, LOW);

  Serial.println("Inicializando...");

  while (!bar.begin()) {
    Serial.println("Barômetro não encontrado. Verifique as conexões!");
    delay(3000);
  }
  Serial.println("Barômetro inicializado!");

  for (int j = 0; j < 4; j++) {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }

  while (!SD.begin(CS)) {
    Serial.println("Falha na inicialização do cartão SD!");
    delay(3000);
  }
  Serial.println("Cartão SD pronto!");

  file = SD.open("DADOS.txt", FILE_WRITE);
  if (file) {
    file.println("----------------------- SISTEMA INICIALIZADO -----------------------");
    file.println("Clock, altitude, pressão, temperatura, hm, vm, hmax:");
    file.close();
  }

  for (int j = 0; j < 3; j++) {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }

  calib();
}

void loop() {
  if (!voo) {
    solo();
  } else {
    flight();
  }
}

void calib() {
  time = millis();
  for (i = 0; i < M; i++) {
    h[i] = bar.readAltitude(H);
    hm += h[i] / M;
  }
  for (i = 0; i < M; i++) {
    float alt = bar.readAltitude(H);
    v[i] = (alt - hm) * 1000 / (millis() - time);
    vm += v[i] / M;
    time = millis();
    delay(75);
    hm -= h[i] / M;
    h[i] = alt;
    hm += h[i] / M;
  }
  hsolo = hm;
  hmax = hm;
  i = 0;
}

void solo() {
  digitalWrite(LED, HIGH);
  digitalWrite(SKIB, LOW);

  float alt = bar.readAltitude(H);
  vm -= v[i] / M;
  v[i] = (alt - hm) * 1000 / (millis() - time);
  vm += v[i] / M;

  Serial.println("----- MODO SOLO -----");
  Serial.print("ALTURA: ");
  Serial.print(alt);
  Serial.print(" m | HM: ");
  Serial.print(hm);
  Serial.print(" m | HSOLO: ");
  Serial.print(hsolo);
  Serial.print(" m | VM: ");
  Serial.print(vm);
  Serial.println(" m/s");

  if (hm - hsolo >= Hmin && fabs(vm) >= 1) {
    Serial.println(">>> DECOLAGEM DETECTADA <<<");
    voo = 1;
  }

  file = SD.open("DADOS.txt", O_WRITE | O_APPEND);
  if (file) {
    file.print((float) millis() / 1000.0);
    file.print(',');
    file.print(alt);
    file.print(',');
    file.print(bar.readPressure());
    file.print(',');
    file.print(bar.readTemperature());
    file.print(',');
    file.print(hm);
    file.print(',');
    file.println(vm);
    file.close();
  }

  time = millis();
  delay(75);
  hm -= h[i] / M;
  h[i] = alt;
  hm += h[i] / M;
  if (hm > hmax) hmax = hm;

  i++;
  if (i >= M) i = 0;

  if (millis() - lastbeep >= 60000) {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    lastbeep = millis();
  }
}


void flight() {
  digitalWrite(LED, LOW);
  digitalWrite(SKIB, LOW);

  float alt = bar.readAltitude(H);
  vm -= v[i] / M;
  v[i] = (alt - hm) * 1000 / (millis() - time);
  vm += v[i] / M;
  time = millis();

  Serial.println("----- MODO VOO -----");
  Serial.print("ALTURA: ");
  Serial.print(alt);
  Serial.print(" m | HM: ");
  Serial.print(hm);
  Serial.print(" m | HMAX: ");
  Serial.print(hmax);
  Serial.print(" m | VM: ");
  Serial.print(vm);
  Serial.println(" m/s");

  while (millis() - time < 75) {
    file = SD.open("DADOS.txt", O_WRITE | O_APPEND);
    if (file) {
      file.print((float) millis() / 1000.0);
      file.print(',');
      file.print(alt);
      file.print(',');
      file.print(bar.readPressure());
      file.print(',');
      file.print(bar.readTemperature());
      file.print(',');
      file.print(hm);
      file.print(',');
      file.print(vm);
      file.print(',');
      file.println(hmax);
      file.close();
    }
  }

  hm -= h[i] / M;
  h[i] = alt;
  hm += h[i] / M;

  if (hmax > hm && hmax - hm < 5 && hmax - hsolo > Hmin_rec && fabs(vm) < Vmin) {
    digitalWrite(SKIB, HIGH);
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    Serial.println(">>> REC ACIONADA <<<");

    file = SD.open("DADOS.txt", FILE_WRITE);
    if (file) {
      file.println("--------------- REC ACIONADA ---------------");
      file.close();
    }
  } else {
    digitalWrite(SKIB, LOW);
  }

  // Removido o retorno automático ao modo solo
  if (hm > hmax) hmax = hm;

  i++;
  if (i >= M) i = 0;
}
