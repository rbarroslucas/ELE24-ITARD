#include <Wire.h>
#include "MPU9250.h"

MPU9250 imu[4];

int setMPU() {
  Wire.begin();
  Wire2.begin();

  if (!imu[0].setup(0x68, MPU9250Setting(), Wire)) {
    Serial1.println("IMU0 ERR");
    return -1;
  }
  if (!imu[1].setup(0x69, MPU9250Setting(), Wire)) {
    Serial1.println("IMU1 ERR");
    return -2;
  }
  if (!imu[2].setup(0x68, MPU9250Setting(), Wire2)) {
    Serial1.println("IMU2 ERR");
    return -3;
  }
  if (!imu[3].setup(0x69, MPU9250Setting(), Wire2)) {
    Serial1.println("IMU3 ERR");
    return -4;
  }

  return 0;
}

void readMPU(char MpuNo) {
  if (imu[MpuNo].update()) {
    Serial1.print(imu[MpuNo].getAccX()); Serial1.print('\t');
    Serial1.print(imu[MpuNo].getAccY()); Serial1.print('\t');
    Serial1.print(imu[MpuNo].getAccZ()); Serial1.print('\t');

    Serial1.print(imu[MpuNo].getGyroX()); Serial1.print('\t');
    Serial1.print(imu[MpuNo].getGyroY()); Serial1.print('\t');
    Serial1.print(imu[MpuNo].getGyroZ()); Serial1.print('\t');

    Serial1.print(imu[MpuNo].getMagX()); Serial1.print('\t');
    Serial1.print(imu[MpuNo].getMagY()); Serial1.print('\t');
    Serial1.print(imu[MpuNo].getMagZ()); Serial1.print('\t');

    Serial1.println(imu[MpuNo].getTemperature());
  }
}
