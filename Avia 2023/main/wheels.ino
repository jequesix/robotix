#include <CrcLib.h>
using namespace Crc;

class Wheels {
  public:
    byte fLPin;
    byte fRPin;
    byte bLPin;
    byte bRPin;

    #define throBind ANALOG::JOYSTICK1_Y
    #define sideBind ANALOG::JOYSTICK1_X
    #define rotaBind ANALOG::JOYSTICK2_X


    Wheels(byte frontLeftPin, byte frontRightPin, byte backLeftPin, byte backRightPin) {
      fLPin = frontLeftPin;
      fRPin = frontRightPin;
      bLPin = backLeftPin;
      bRPin = backRightPin;
      
      CrcLib::InitializePwmOutput(fLPin);
      CrcLib::InitializePwmOutput(fRPin);
      CrcLib::InitializePwmOutput(bLPin);
      CrcLib::InitializePwmOutput(bRPin);  
    }

    void Update() {
      CrcLib::MoveHolonomic(throBind, sideBind, rotaBind, fLPin, bLPin, fRPin, bRPin);
    }
};
