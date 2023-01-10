#include <CrcLib.h>
using namespace Crc;

class Wheels {
  public:
    #define CRC_WPM_1 fLPin;
    #define CRC_PWM_2 fRPin;
    #define CRC_PWM_3 bLPin;
    #define CRC_PWM_4 bRPin;

    #define ANALOG::JOYSTICK1_Y throBind;
    #define ANALOG::JOYSTICK1_X turnBind;
    #define ANALOG::JOYSTICK2_Y rotaBind


    Wheels(int frontLeftPin, int frontRightPin, int backLeftPin, int backRightPin) {
      fLPin = frontLeftPin;
      fRPin = frontRightPin;
      bLPin = backLeftPin;
      bRPin = backRightPin;
      
      CrcLib::InitializePwmOutput(fLPin);
      CrcLib::InitializePwmOutput(fRPin);
      CrcLib::InitializePwmOutput(bLPin);
      CrcLib::InitializePwmOutput(bRPin);  
    }

    Update() {
      CrcLib::MoveHolonomic(throBind, turnBind, rotaBind, fLPin, bLPin, fRPin, bRPin);
    }
};
