#include "gp.h"
#include <CrcLib.h>

GPElevator *gpElv;
GPElevator *gpElv2;

void setup() {
  CrcLib::Initialize();

  GPElevator::config gpElvConf;
  gpElvConf.manualControl = ANALOG::JOYSTICK1_Y;  gpElvConf.resetButton = BUTTON::L1;  gpElvConf.offButton = BUTTON::HATL;                             
  gpElvConf.motorControlPin = CRC_PWM_1;          gpElvConf.encoderPin1 = CRC_ENCO_A;  gpElvConf.encoderPin2 = CRC_ENCO_B;
  gpElvConf.speed = 15;                           gpElvConf.accelerationGap = 10;      gpElvConf.spdMultiplier = 1;
  gpElv = new GPElevator(gpElvConf);

  GPElevator::config gpElvConf2;
  gpElvConf2.manualControl = ANALOG::JOYSTICK1_Y;  gpElvConf2.resetButton = BUTTON::L1;   gpElvConf2.offButton = BUTTON::HATL;                             
  gpElvConf2.motorControlPin = CRC_PWM_2;          gpElvConf2.encoderPin1 = CRC_I2C_SDA;  gpElvConf2.encoderPin2 = CRC_I2C_SCL;
  gpElvConf2.speed = 15;                           gpElvConf2.accelerationGap = 10;       gpElvConf2.spdMultiplier = -1;
  gpElv2 = new GPElevator(gpElvConf2);
  
  Serial.begin(2000000);
}

bool started = false;
void loop() {
  CrcLib::Update();

  if (!started) {
    delay(3000);
    started = true;
  }

  //if (CrcLib::IsCommValid()) {
    gpElv->Update();
    gpElv2->Update();
  // }
}
