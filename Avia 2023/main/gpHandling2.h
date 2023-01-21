#include <CrcLib.h>
#include "time.h"
using namespace Crc;

class GPCapture {
  private:
    const int mRecMPin;
    const CrcUtility::BUTTON mRecMBind;
    Time motorTime(2000)

  public:
    Capture(int receiverMotorPin, CrcUtility::BUTTON receiverMotorBinding) : mRecMPin(receiverMotorPin), mRecMBind(receiverMotorBinding) {}

    void Setup() {
      //Initialize Crc Functions
      CrcLib::InitializePwmOutput(mRecMPin);
    }

    void Update() {
      if (CrcLib::ReadDigitalChannel(mRecMBind) == 1) {
        while (motorTime.singleState() == false) {
          CrcLib::SetPwmOutput(mRecMPin, 50);
        }
        
      }
    }
};

class GPColor {
  private:
    const int _lSPin;
    const int _cSPin;
    const int _fMPin;
    const int _fPin;

    const int _tColor;
    int _gpColor;
  public:
    // teamColor: blue = 0, yellow = 1
    GPColor(int laserSensorPin, int colorSensorPin, int forwardMotorPin, int flipperPin, int teamColor) : 
      _lSPin(laserSensorPin), _cSPin(colorSensorPin), _tColor(teamColor), _fMPin(forwardMotorPin), _fPin(flipperPin) {}

    void Setup() {
      pinMode(_lSPin, INPUT);
      pinMode(_cSPin, INPUT);
      pinMode(_fMPin, OUTPUT);
      pinMode(_fPin, OUTPUT);    
    }

    void Update() {
      if (CrcLib:GetDigitalInput(_lSPin)) {
        CrcLib::SetDigitalOutput(_fMPin, 50);
      }
      
    }
}
