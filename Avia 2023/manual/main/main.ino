#include "wheels.h"
#include "gp.h"

#include <CrcLib.h>
using namespace Crc;

Wheels whl(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_X, ANALOG::JOYSTICK1_X, 4, 5, 6, 7);
GPGround gpGnd(ANALOG::GACHETTE_L, ANALOG::GACHETTE_R, 8, 50, 30, 9);
GPElevator gpElv(BUTTON::eleUBinding, BUTTO::eleDBinding, int motorControlPin, uint8_t motorPin1, uint8_t motorPin2, int targetSpeed, int accelerationGap, int stepNumber, int stepsPosition[]);
GPGrabber gpGrb();

void setup() {
  CrcLib::Initialize();

  whl.Setup();
  gpGnd.Setup();
}

void loop() {
  CrcLib::Update();

  whl.Update();
  gpGnd.Update();
}
