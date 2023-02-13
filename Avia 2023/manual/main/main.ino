#include "wheels.h"
#include "gp.h"

#include <CrcLib.h>
using namespace Crc;

Wheels whl(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_X, ANALOG::JOYSTICK1_X, 4, 5, 6, 7);
GPGround gpGnd(ANALOG::GACHETTE_L, ANALOG::GACHETTE_R, 11, 5, 50, 30, BUTTON::SELECT, BUTTON::START, BUTTON::LOGO, 13, -128, 127, -10);
GPElevator gpElv(BUTTON::HATL, BUTTON::COLORS_UP, BUTTON::COLORS_RIGHT, BUTTON::COLORS_DOWN, BUTTON::COLORS_LEFT, BUTTON::ARROW_UP, BUTTON::ARROW_RIGHT, BUTTON::ARROW_DOWN, 9, 2, 3, 70, 7, );
GPGrabber gpGrb();

void setup() {
  CrcLib::Initialize();

  whl.Setup();
  gpGnd.Setup();
  gpElv.Setup();
}

void loop() {
  CrcLib::Update();

  whl.Update();
  gpGnd.Update();
  gpElv.Update();
}
