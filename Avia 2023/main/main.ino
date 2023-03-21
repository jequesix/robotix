#include "wheels.h"
#include "gp.h"

#include <CrcLib.h>
using namespace Crc;

// 0 = blue, 1 = yellow
int teamColor = 0;

Wheels whl(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_X, ANALOG::JOYSTICK1_X, CRC_PWM_8, CRC_PWM_1, CRC_PWM_7, CRC_PWM_2, 50);

GPGround *gpGnd;
GPElevator *gpElv;
GPGrabber *gpGrb;


void setup() {
  CrcLib::Initialize();

  GPGround::config gpGndConf;
  gpGndConf.teamColor = teamColor;
  gpGndConf.captureBinding = ANALOG::GACHETTE_L;     gpGndConf.captureReverseBinding = ANALOG::GACHETTE_R;
  gpGndConf.leftCaptureMotorPin = CRC_PWM_3;         gpGndConf.rightCaptureMotorPin = CRC_PWM_5;    gpGndConf.colorSensorPin = CRC_ANA_1;         
  gpGndConf.captureLaserSensorPin = CRC_ANA_2;       gpGndConf.captureMotorsSpeed = 127;            gpGndConf.captureMotorsSlowSpeed = 40;
  gpGndConf.flipperServoPin = CRC_PWM_9;             gpGndConf.flipperLaserSensorPin = CRC_ANA_3;
  gpGndConf.flipperNeutralBinding = BUTTON::SELECT;  gpGndConf.flipperFlipBinding = BUTTON::START;  gpGndConf.flipperSkipBinding = BUTTON::LOGO;
  gpGndConf.flipperNeutralPosition = -105;           gpGndConf.flipperFlipPosition = 127;           gpGndConf.flipperSkipPosition = 50;
  gpGndConf.elevatorLaserSensorPin = CRC_ANA_4;
  gpGnd = new GPGround(gpGndConf);

  GPElevator::config gpElvConf;
  gpElvConf.manualToggle = BUTTON::HATR;      gpElvConf.manualControl = ANALOG::JOYSTICK2_Y;  gpElvConf.resetButton = BUTTON::HATL;         gpElvConf.offButton = BUTTON::ARROW_LEFT;
  gpElvConf.step1Button = BUTTON::COLORS_UP;  gpElvConf.step2Button = BUTTON::COLORS_RIGHT;   gpElvConf.step3Button = BUTTON::COLORS_DOWN;  gpElvConf.step4Button = BUTTON::COLORS_LEFT;
  gpElvConf.step5Button = BUTTON::ARROW_UP;   gpElvConf.step6Button = BUTTON::ARROW_RIGHT;    gpElvConf.step7Button = BUTTON::ARROW_DOWN;
  gpElvConf.step1Position = 0;                gpElvConf.step2Position = 1000;                 gpElvConf.step3Position = 2500;               gpElvConf.step4Position = 4500;                
  gpElvConf.step5Position = 5900;             gpElvConf.step6Position = 8000;                 gpElvConf.step7Position = 9700;
  gpElvConf.motorControlPin = CRC_PWM_4;      gpElvConf.encoderPin1 = CRC_ENCO_A;             gpElvConf.encoderPin2 = CRC_ENCO_B;
  gpElvConf.speed = 15;                       gpElvConf.accelerationGap = 10;
  gpElv = new GPElevator(gpElvConf);

  GPGrabber::config gpGrbConf;
  gpGrbConf.servoBinding = BUTTON::R1;
  gpGrbConf.servo1Pin = CRC_PWM_11;     gpGrbConf.servo1Position_1 = -40;   gpGrbConf.servo1Position_2 = -40;   gpGrbConf.servo1Position_3 = 127;
  gpGrbConf.servo2Pin = CRC_PWM_10;     gpGrbConf.servo2Position_1 = -2;  gpGrbConf.servo2Position_2 = -2;  gpGrbConf.servo2Position_3 = -128;
  gpGrbConf.wheelBinding = BUTTON::L1;  gpGrbConf.wheelPin = CRC_PWM_6;   gpGrbConf.wheelSpeed = 50;
  gpGrb = new GPGrabber(gpGrbConf);
  
  whl.Setup();
  Serial.begin(2000000);
}

void loop() {
  CrcLib::Update();

  if (CrcLib::IsCommValid()) {
    whl.Update();

    gpGnd->Update();
    gpElv->Update();
    gpGrb->Update();
  }
}

//Neopixel = CRC_PWM_12
