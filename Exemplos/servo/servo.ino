#include <ESP32Servo.h> 
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Servo servo;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  
  servo.attach(32);

  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("Erro ao iniciar MPU6050!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int value = a.acceleration.y;
  value = map(value, -10, 10, 180, 0);

  value = constrain(value, 0, 180);

  servo.write(value);
  
  Serial.println(value);
  delay(10);
}
