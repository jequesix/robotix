#include "wheels.h"
#include "gp.h"

#include <CrcLib.h>
using namespace Crc;

Wheels whl(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_X, ANALOG::JOYSTICK1_X, CRC_PWM_1, CRC_PWM_3, CRC_PWM_4, CRC_PWM_7);
GPGround gpGnd(ANALOG::GACHETTE_L, ANALOG::GACHETTE_R, CRC_PWM_8, CRC_PWM_2, 50, 30, BUTTON::SELECT, BUTTON::START, BUTTON::LOGO, CRC_PWM_10, -128, 127, -10);
GPElevator gpElv(BUTTON::HATL, BUTTON::COLORS_UP, BUTTON::COLORS_RIGHT, BUTTON::COLORS_DOWN, BUTTON::COLORS_LEFT, BUTTON::ARROW_UP, BUTTON::ARROW_RIGHT, BUTTON::ARROW_DOWN, CRC_PWM_6, CRC_ENCO_A, CRC_ENCO_B, 70, 7, 10, 20, 30, 40, 50, 60, 70);
GPGrabber *gpGrb;

void setup() {
  CrcLib::Initialize();

  GPGrabber::config gpGrbConf;
  gpGrbConf.servoBinding = BUTTON::R1;
  gpGrbConf.servo1Pin = CRC_PWM_11;     gpGrbConf.servo1Position_1 = 0;   gpGrbConf.servo1Position_2 = 90;   gpGrbConf.servo1Position_3 = 180;
  gpGrbConf.servo2Pin = CRC_PWM_12;     gpGrbConf.servo2Position_1 = 10;  gpGrbConf.servo2Position_2 = 135;  gpGrbConf.servo2Position_3 = 140;
  gpGrbConf.wheelBinding = BUTTON::L1;  gpGrbConf.wheelPin = CRC_PWM_9;   gpGrbConf.wheelSpeed = 15;
  gpGrb = new GPGrabber(gpGrbConf);
  
  whl.Setup();
  gpGnd.Setup();
  gpElv.Setup();
  gpGrb->Setup();
}

void loop() {
  CrcLib::Update();

  if (CrcLib::IsCommValid()) {
    whl.Update();
    gpGnd.Update();
    gpElv.Update();
    gpGrb->Update();
  }
}
