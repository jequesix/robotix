#include "gp.h"
#include <CrcLib.h>

GPElevator *gpElv;
GPElevator *gpElv2;
RotateArm *rttArm;

void setup() {
  CrcLib::Initialize();

  GPElevator::config gpElvConf;
  gpElvConf.manualControl = ANALOG::JOYSTICK1_Y;  gpElvConf.resetButton = BUTTON::L1;  gpElvConf.offButton = BUTTON::HATL;                             
  gpElvConf.motorControlPin = CRC_PWM_1;          gpElvConf.encoderPin1 = CRC_ENCO_A;  gpElvConf.encoderPin2 = CRC_ENCO_B;
  gpElvConf.speed = 15;                           gpElvConf.accelerationGap = 10;
  gpElv = new GPElevator(gpElvConf);

  GPElevator::config gpElvConf2;
  gpElvConf.manualControl = ANALOG::JOYSTICK1_Y;  gpElvConf.resetButton = BUTTON::L1;  gpElvConf.offButton = BUTTON::HATL;                             
  gpElvConf.motorControlPin = CRC_PWM_2;          gpElvConf.encoderPin1 = CRC_SERIAL_TXD1;    gpElvConf.encoderPin2 = CRC_SERIAL_RXD1;
  gpElvConf.speed = 15;                           gpElvConf.accelerationGap = 10;
  gpElv2 = new GPElevator(gpElvConf2);

  RotateArm::config rotateArmConf;
  rotateArmConf.rotateMotorPin = CRC_PWM_12;   rotateArmConf.servo1Pin = CRC_PWM_3;  rotateArmConf.servo2Pin = CRC_PWM_4;
  rotateArmConf.raiseArmButton = ANALOG::JOYSTICK2_X;
  rttArm = new RotateArm(rotateArmConf);
  
  Serial.begin(2000000);
}

void loop() {
  CrcLib::Update();

  if (CrcLib::IsCommValid()) {
    gpElv->Update();
    rttArm->Update();
    
  }
}
