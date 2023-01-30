#include <CrcLib.h>
#include <Encoder.h>
#include "time.h"

using namespace Crc;


class Commands {
  private:
    _pressed = false;

    bool isPressed(button) {
      if (CrcLib::ReadDigitalChannel(button) == 0) {
        if (_pressed == false) {
          return false;
        } else if (_pressed == true) {
          _pressed = false;
          return false;
        }        
      } else if {CrcLib::ReadDigitalChannel(button) == 1) {
         if (_pressed == true) {
          return false;
         } else if (_pressed == false) {
          _pressed = true;
          return true;
         }
          
      }
      if () {
        _pressed = true;
        return true;
      } else if (_pressed == true && CrcLib::ReadDigitalChannel(button) == 0) {
        _pressed = false;
        return false;
      } else {
        return false;
      }
    }
};

class GPFlip: private Commands {
  private:
    // 0 = auto, 1 = manual
    int _mode = 0;
    const CrcUtility::BUTTON _modeBind;

    // Capture component
    const int _lasSPin;
    int _lasSState;

    const int _entMPin;
    const int _forMPin;

    const CrcUtility::BUTTON _capBind;
    const int _capMSpeed;
    
    // Flipper component
    // 0 = blue, 1 = yellow
    const int _tColor = 0;
    int _gpColor;

    const int _fliMCPin;
    const int _fliMPin1;
    const int _fliMPin2;

    const int _fliMSpeed;

  public:
    GPFlip(int teamColor, CrcUtility::BUTTON modeBinding, CrcUtility::BUTTON captureBinding, int laserSensorPin, int entryMotorPin, int forwardMotorPin, int captureMotorsSpeed, int flipperMotorControlPin, int flipperMotorPin1, int flipperMotorPin2, int flipperMotorSpeed) : 
      _tColor(teamColor), _modeBind(modeBinding), _capBind(captureBinding), _lasSPin(laserSensorPin), _entMPin(entryMotorPin), _forMPin(forwardMotorPin), _capMSpeed(captureMotorsSpeed), _fliMCPin(flipperMotorControlPin), _fliMPin1(flipperMotorPin1), _fliMPin2(flipperMotorPin2), _fliMSpeed(flipperMotorSpeed) {
        Encoder flipper(_fliMPin1, _fliMPin2);
      }

    void Setup() {
      
      //Initialize Crc Functions
      CrcLib::InitializePwmOutput(_entMPin);
      CrcLib::InitializePwmOutput(_forMPin);
    }

    void Update() {
      _lasSState = CrcLib::GetDigitalInput(_lasSPin);

      if (isPressed(_modeBind)) {
        if (_mode == 0) {
          _mode = 1;
        } else if (_mode == 1) {
          _mode = 0;
        }
      }

      if (_mode = 0 && _lasSState == 0) {
        switch(motorTime.cycleState())
          case 1:
            CrcLib::SetPwmOutput(_entMPin, _capMSpeed);
            CrcLib::SetPwmOutput(_forMPin, _capMSpeed);
            break;
          case 2:
            CrcLib::SetPwmOutput(_entMPin, 0);
            CrcLib::SetPwmOutput(_forMPin, 0);

            if (_tColor != _gpColor)

            if (flipper.read() < 500) {
              CrcLib::SetPwmOutput(_fliMCPin, _fliMSpeed);   
            } else {
              CrcLib::SetPwmOutput(_fliMCPin, 0);
            }
            
      }
    }
};

class GPElevator: private Commands {
  private:
    const int _lSPin;
    const int _cSPin;
    const int _fMPin;
    const int _fPin;

    int _

    const int _tColor;
    int _gpColor;

    bool _fMTrigger;
  public:
    // teamColor: blue = 0, yellow = 1
    GPColor(int laserSensorPin, int colorSensorPin, int forwardMotorPin, int teamColor) :
      _lSPin(laserSensorPin), _cSPin(colorSensorPin), _fMPin(forwardMotorPin), _tColor(teamColor) 
        {
        Time motorTime(2000);
        Encoder flipper(CRC_ENCO_A, CRC_ENCO_B);
        }

    void Setup() {
      pinMode(_lSPin, INPUT);
      pinMode(_cSPin, INPUT);
      pinMode(_foMPin, OUTPUT);    
    }

    void Update() {
      if (CrcLib::ReadDigitalChannel(mRecMBind) == 1) {
        if (motorTime.singleState() == false) {
          CrcLib::SetPwmOutput(mRecMPin, 50);
        }     
      }

      switch(motorTime.cycleState())
        case 1:
          CrcLib::SetPwmOutput(_foMPin, 50);
          break;
        case 2:
          int fPos = flipper.read();
          if (fPos < 180) {
            CrcLib::SetPwmOutput(_flMPin, 50)
          } else {
            CrcLib::SetPwmOutput(_flMPin, 0)
          }

      
         
      }
      
    }
}
