#include "wheels2.h"
#include "gpHandling2.h"
#include <CrcLib.h>

using namespace Crc;

// Global
Wheels sMyWheels(CrcUtility::ANALOG::JOYSTICK1_Y, CrcUtility::ANALOG::JOYSTICK1_X, CrcUtility::ANALOG::JOYSTICK2_X, 1, 2, 3, 4);
Capture sMyCapture(5, CrcUtility::BUTTON::HATR);
Time sMyLedBlinky(500, 3);

void setup() {
  CrcLib::Initialize();

  sMyWheels.Setup();
  sMyCapture.Setup();
}


void loop() {
  CrcLib::Update();

  sMyWheels.Update();
  sMyCapture.Update();



  int action;
  sMyLedBlinky.CycleAction(action);
  switch(action)
  {
    case 1:
    //LED BLUE off
    // LED RED on    
    break;

    case 2:
    // LED RED off
    //LED GREEN on
    break;

    case 3:
    //LED GREEN off
    //LED BLUE on    
    break;

    default:
    // Do nothing
    break;
  }
}
