#include "gp.h"
#include <CrcLib.h>

GPElevator *gpElv;
GPElevator *gpElv2;

void setup() {
  CrcLib::Initialize();

  GPElevator::config gpElvConf;
  gpElvConf.manualControl = ANALOG::JOYSTICK1_Y;  gpElvConf.resetButton = BUTTON::L1;  gpElvConf.offButton = BUTTON::HATL;                             
  gpElvConf.motorControlPin = CRC_PWM_4;          gpElvConf.encoderPin1 = CRC_ENCO_A;  gpElvConf.encoderPin2 = CRC_ENCO_B;
  gpElvConf.speed = 15;                           gpElvConf.accelerationGap = 10;
  gpElv = new GPElevator(gpElvConf);

  GPElevator::config gpElvConf2;
  gpElvConf.manualControl = ANALOG::JOYSTICK1_Y;  gpElvConf.resetButton = BUTTON::L1;  gpElvConf.offButton = BUTTON::HATL;                             
  gpElvConf.motorControlPin = CRC_PWM_4;          gpElvConf.encoderPin1 = CRC_ENCO_A;  gpElvConf.encoderPin2 = CRC_ENCO_B;
  gpElvConf.speed = 15;                           gpElvConf.accelerationGap = 10;
  gpElv2 = new GPElevator(gpElvConf2);
  
  Serial.begin(2000000);
}

void loop() {
  CrcLib::Update();

  if (CrcLib::IsCommValid()) {
    gpElv->Update();
  }
}
