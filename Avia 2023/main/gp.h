#include <CrcLib.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"

using namespace Crc;


class CommandButton {
private:
    const CrcUtility::BUTTON _bind;
    bool _pressed = false;

public:
    bool wasClicked() {
      switch (CrcLib::ReadDigitalChannel(_bind)) {
        case 0:
          if (_pressed == false) {
            return false;
          } else {
            _pressed = false;
            return false;
          }
        case 1:
          if (_pressed == true) {
            return false;
          } else {
            _pressed = true;
            return true;
          }     
      }
    }

   bool isPressed() {
      return CrcLib::ReadDigitalChannel(_bind);
    }

    CommandButton(CrcUtility::BUTTON binding) : _bind(binding) {}
      
};

class GPGround {
  private:
    // 0 = auto, 1 = manual
    int _mode = 0;
    CommandButton _modeButton;

    // Capture component
    const int _entMPin;
    const int _lasSPin;
    unsigned char _lasSState;
    bool _isGp;
    int _colSPin;
    const int _colSLPin;
    const int _colSLNum;
    bool _colScanned;
    // 0 = blue, 1 = yellow
    const int _tColor = 0;
    const int _blue = 500;
    const int _yellow = 200;
    const int _colInterval = 10;
    int _colValues[2];
    const CrcUtility::BUTTON _capBind;
    const CrcUtility::BUTTON _capRBind;
    const int _capMSpeed;
    
    // Flipper component
    const int _fliMCPin;
    const int _fliMPin1;
    const int _fliMPin2;
    const int _fliMSpeed;
    const int _fliSMSpeed;
    const int _fliNPos;
    const int _fliFPos;
    const int _fliSPos;
    int _fliState;
    const CrcUtility::BUTTON _fliBind;
    const int _fliInterval = 10;

    Time _cSTime;
    Time _motorTime;
    Encoder _flipper;
    Adafruit_NeoPixel cSLed;





    void gpCapture(int motorSpeed) {
      CrcLib::SetPwmOutput(_entMPin, motorSpeed);
    }


    int gpFindColor() {
      int gpColVal;
      if (_colScanned == false) {
        int cSState = CrcLib::GetAnalogInput(_colSPin);

        switch(_cSTime.cycleState()) {
          case 2:
            _colValues[0] = cSState;
            break;
          case 3:
            _colValues[1] = cSState;
            gpColVal = (_colValues[0] + _colValues[1]) / 2;
            _colScanned = true;
            break;
        }

      }   
      if (gpColVal >= _blue - _colInterval && gpColVal <= _blue + _colInterval) {
        return 0;
      } else if (gpColVal >= _yellow - _colInterval && gpColVal <= _yellow + _colInterval) {
        return 1;
      } else {
        return 2;
      }
    }

    void gpFlip(int pos) {
      CrcLib::SetPwmOutput(_fliMCPin, pos);
      }





  public:
    GPGround(int teamColor, CrcUtility::BUTTON modeBinding, CrcUtility::BUTTON captureBinding, int laserSensorPin, int colorSensorPin, int colorSensorLedPin, int entryMotorPin, int captureMotorsSpeed, int flipperMotorControlPin, int flipperMotorPin1, int flipperMotorPin2, int flipperMotorSpeed) : 
      _tColor(teamColor), _modeButton(modeBinding), _capBind(captureBinding), _lasSPin(laserSensorPin), _colSPin(colorSensorPin), _colSLPin(colorSensorLedPin), _entMPin(entryMotorPin), _capMSpeed(captureMotorsSpeed), _fliMCPin(flipperMotorControlPin), _fliMPin1(flipperMotorPin1), _fliMPin2(flipperMotorPin2), _fliMSpeed(flipperMotorSpeed),
      _cSTime(100, 3), _motorTime(2000, 3), _flipper(_fliMPin1, _fliMPin2)
      {
        Adafruit_NeoPixel cSLed(int _intcolSLNum, int _colSLPin);
        _fliState = 0;
      }

    void Setup() {
      cSLed.begin();
      for (int led = 0; led < (_colSLNum -1); led++) {
        cSLed.setPixelColor(led, 255, 255, 255);
      }
      cSLed.show();

      CrcLib::InitializePwmOutput(_entMPin);
    }

    void Update() {
      static int gpColor = 0;   // or, use a member of the class

      if (_modeButton.wasClicked()) {
        switch (_mode) {
          case 0:
            _mode = 1;
          case 1:
            _mode = 0;
        }
      }

    // Automatic
      if (_mode == 0) {
          _lasSState = CrcLib::GetDigitalInput(_lasSPin);
          if (_lasSState == LOW) {
            _isGp = true;
          }

          if (_isGp) {
            switch(_motorTime.cycleState()) {
              case 1:
                gpColor = gpFindColor();  // check color
                gpCapture(_capMSpeed);    // capture game piece
                break;
              case 2:
                gpCapture(0);            // captured !
                if (gpColor == 2 || _tColor == gpColor) {  // if no color found OR color is already our team color
                  gpFlip(_fliSPos);                            // do nothing
                } else if (_tColor != gpColor) {           // wrong color
                  gpFlip(_fliFPos);                            // flip it
                }
                break;
              case 3:
                gpFlip(_fliNPos);
                _colScanned = false;
                _isGp = false;
                break;
            }
          }
      }

      // Manual
      else {
        if (CrcLib::ReadDigitalChannel(_capBind) == 0) {
          switch (CrcLib::ReadDigitalChannel(_capRBind)) {
            case 0:
              gpCapture(0);
            case 1:
              gpCapture(_capMSpeed*(-1));
          }
        }

        if (CrcLib::ReadDigitalChannel(_fliBind) == 1) {
          _fliState++;
          if (_fliState > 3) {
            _fliState = 1;
          }
        }
      }


      switch (_fliState) {
        case 1:
          gpFlip(_fliNPos);
        case 2:
          gpFlip(_fliFPos);
        case 3:
          gpFlip(_fliSPos);
      }
    }
};


//--------------------------------------------------------------------
//--------------------------------------------------------------------
class GPElevator {
  private:
    const int _eleMCPin;
    const int _eleMPin1;
    const int _eleMPin2;
    const int _eleTSpeed;
    const int _eleAGap;
    const int _stepNum;
    int *_stepPos;
    CommandButton _eleUButton;
    CommandButton _eleDButton;
    
    const int _spdIncr = 1;

    int _curSpdIncr;
    int _curTgtSpeed;
    int _curSpeed;
    int _sarPos;
    int _tarPos;
    int _curPos;
    int _curStep = 0;

    void elvMove(bool reverse) {
      if (reverse) {
        _curTgtSpeed = _eleTSpeed*(-1);
        _curSpdIncr = _spdIncr*(-1);
      }
    }




  public:
    GPElevator(CrcUtility::BUTTON eleUBinding, CrcUtility::BUTTON eleDBinding, int motorControlPin, int motorPin1, int motorPin2, int targetSpeed, int accelerationGap, int stepNumber, int stepsPosition[]) :
    _eleUButton(eleUBinding), _eleDButton(eleDBinding), _eleMCPin(motorControlPin), _eleMPin1(motorPin1), _eleMPin2(motorPin2), _eleTSpeed(targetSpeed), _eleAGap(accelerationGap), _stepNum(stepNumber)
      {
        Encoder elevator(_eleMPin1, _eleMPin2);
        _stepPos = new int[_stepNum];
        for (int i=0; i<_stepNum ; i++)
        {
          _stepPos[i] = stepsPosition[i];
        }
      }

    void Setup() {
      CrcLib::InitializePwmOutput(_eleMCPin);

    }

    void Update() {
      if (_eleUButton.wasClicked()) {
        if (_curStep < (_stepNum - 1)) {
          _curStep++;
        }
      } else if (_eleDButton.wasClicked()) {
        if (_curStep > 0) {
          _curStep--;
        }
      }

      _tarPos = _stepPos[_curStep];
      ////////////_staPos = elevator.read();


    }
};



//--------------------------------------------------------------------
//--------------------------------------------------------------------
class GPGrabber {
  private:
    CommandButton _graSrvButton;
    CommandButton _graWhlButton;
    Time _whlTime;
    const int _srv1Pin;
    int _srv1Pos[3];
    const int _srv2Pin;
    int _srv2Pos[3];
    const int _whlPin;
    const int _whlSpeed;

    const int _stepNum = 3;
    int _step = 0;
    bool _isWhlSpin = false;


    void srvMove() {
      if (_step < 3)
      {
        CrcLib::SetPwmOutput(_srv1Pin, _srv1Pos[_step]);
        CrcLib::SetPwmOutput(_srv2Pin, _srv2Pos[_step]);
      }
    }

    void whlSpin(int whlSpeed, bool reverse = false) {
      if (reverse) {
        whlSpeed = whlSpeed*(-1);
      }

      CrcLib::SetPwmOutput(_whlPin, whlSpeed);
    }

   public:
    GPGrabber(CrcUtility::BUTTON graSrvBinding, CrcUtility::BUTTON _graWhlBind, int servo1Pin, int servo2Pin, int servo1Positions[3], int servo2Positions[3], int wheelPin, int wheelSpeed, int wheelTimeInterval) :
      _graSrvButton(graSrvBinding), _graWhlButton(_graWhlBind), _whlTime(wheelTimeInterval), _srv1Pin(servo1Pin), _srv2Pin(servo2Pin), _whlPin(wheelPin), _whlSpeed(wheelSpeed)
      {
        for (int i=0; i<3; i++)
        {
          _srv1Pos[i] = servo1Positions[i];
          _srv2Pos[i] = servo2Positions[i];
        }
      }

    void Setup() {
      CrcLib::InitializePwmOutput(_srv1Pin);
      CrcLib::InitializePwmOutput(_srv2Pin);
      CrcLib::InitializePwmOutput(_whlPin);
    }

    void Update() {
      // GRABBER CLAW
      if (_graSrvButton.wasClicked())
      {
        if (_step < (_stepNum - 1)) 
        {
          _step++;
        } else 
        {
          _step = 0;
        }
      }
      srvMove();

      // GRABBER CONVEYOR
      if (_isWhlSpin == false) {
        if (_graWhlButton.isPressed()) {
          _isWhlSpin = true;
          _whlTime.reset();
        }
      }
      else {
          if (_whlTime.singleState()) { // 1: time elapsed.
              whlSpin(0);
              _isWhlSpin = false;            
          }
          else {
            whlSpin(_whlSpeed);
          }
      }
    }
};
