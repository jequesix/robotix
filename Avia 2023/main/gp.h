#include <CrcLib.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"

using namespace Crc;


class Commands {
  private:
    const CrcUtility::BUTTON _eleUBind;
    const CrcUtility::BUTTON _eleDBind;

    const CrcUtility::BUTTON _graSBind;

    bool _pressed = false;

    bool isPressed(button) {
      switch (CrcLib::ReadDigitalChannel(button)) {
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
};

class GPGround: private Commands {
  private:
    // 0 = auto, 1 = manual
    int _mode = 0;
    const CrcUtility::BUTTON _modeBind;

    // Capture component
    const int _entMPin;
    const int _lasSPin;
    unsigned char _lasSState;
    bool _isGp;
    int _colSPin;
    const int _colSLPin;
    // Number of leds - 1
    const int _colSLNum;
    // 0 = blue, 1 = yellow
    const int _tColor = 0;
    const int _blue = 500;
    const int _yellow = 200;
    const int _colInterval = 10;
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
    const CrcUtility::BUTTON _fliBind;
    const int _fliInterval = 10;





    void gpCapture(int motorSpeed) {
      CrcLib::SetPwmOutput(_entMPin, motorSpeed);
    }

    void gpFindColor() {
      if (_colScanned == false) {
        int cSState = CrcLib::GetAnalogInput(_colSPin);

        switch(cSTime.cycleState())
          case 2:
            _colValues[0] = cSState;
            break;
          case 3:
            _colValues[1] = cSState;
            gpColVal = (_colValues[0] + _colValues[1]) / 2
            _colScanned = true;
            break;
      }   
      if (gpColVal >= _blue - _colInterval && gpColVal <= _blue + _colInterval) {
        return 0;
      } else if (gpColVal >= _yellow - _colInterval && gpColVal <= _yellow + _colInterval) {
        return 1;
      } else {
        return 2;
      }
    }

    void gpFlip(int pos, bool reverse) {
      if (flipper.read() >= pos - _fliInterval && flipper.read() <= pos + _fliInterval) {
        CrcLib::SetPwmOutput(_fliMCPin, 0);
        return;
      }

      switch (reverse) {
        case 0:
          if (flipper.read() < pos - _fliInterval) {
            CrcLib::SetPwmOutput(_fliMCPin, _fliMSpeed);
          } else if (flipper.read() > pos + _fliInterval) {
            CrcLib::SetPwmOutput(_fliMCPin, _fliSMSpeed*(-1));
          }
        case 1:
          if (flipper.read() > pos + _fliInterval) {
            CrcLib::SetPwmOutput(_fliMCPin, _fliMSpeed*(-1));
          } else if (flipper.read() < pos - _fliInterval) {
            CrcLib::SetPwmOutput(_fliMCPin, _fliSMSpeed);
          }
      }

    }




  public:
    GPGround(int teamColor, CrcUtility::BUTTON modeBinding, CrcUtility::BUTTON captureBinding, int laserSensorPin, int colorSensorPin,, int colorSensorLedPin, int entryMotorPin, int forwardMotorPin, int captureMotorsSpeed, int flipperMotorControlPin, int flipperMotorPin1, int flipperMotorPin2, int flipperMotorSpeed) : 
      _tColor(teamColor), _modeBind(modeBinding), _capBind(captureBinding), _lasSPin(laserSensorPin), _colSPin(colorSensorPin), _colSLPin(colorSensorLedPin) _entMPin(entryMotorPin), _forMPin(forwardMotorPin), _capMSpeed(captureMotorsSpeed), _fliMCPin(flipperMotorControlPin), _fliMPin1(flipperMotorPin1), _fliMPin2(flipperMotorPin2), _fliMSpeed(flipperMotorSpeed)
      {
        Time cSTime(100, 3);
        Time motorTime(2000, 3);
        Encoder flipper(_fliMPin1, _fliMPin2);
        Adafruit_NeoPixel cSLed(5, _colSLPin, NEO_KHZ800);
      }

    void Setup() {
      cSLed.begin();
      for (int led = 0; led < _colSLNum; led++) {
        cSLed.setPixelColor(led, 255, 255, 255);
      }
      cSLed.show();

      //Initialize Crc Functions
      CrcLib::InitializePwmOutput(_entMPin);
    }

    void Update() {
      if (isPressed(_modeBind)) {
        switch (_mode) {
          case 0:
            _mode = 1;
          case 1:
            _mode = 0;
        }
      }

      switch (_mode)
        case 0:
          _lasSState = CrcLib::GetDigitalInput(_lasSPin);
          if (_lasSState == LOW) {
            _isGp = true;
          }

          if (_isGp) {
            switch(motorTime.cycleState()) {
              case 1:
                int gpColor = gpFindColor();
                gpCapture(_capMSpeed);
                break;
              case 2:
                gpCapture(0);
                if (gpColor == 2 || _tColor == gpColor) {
                  gpFlip(_fliSPos);
                } else if (_tColor != gpColor) {
                  gpFlip(_fliFPos);
                }
                break;
              case 3:
                gpFlip(_fliNPos, 1);
                _colScanned = false;
                _isGp = false;
                break;
            }
          }
          
        case 1:
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

          switch (_fliState) {
            case 1:
              gpFlip(_fliNPos, 1);
            case 2:
              gpFlip(_fliFPos);
            case 3:
              gpFlip(_fliSPos, 1)
          }
    }
};

class GPElevator: private Commands {
  private:
    const int _eleMCPin;
    const int _eleMPin1;
    const int _eleMPin2;
    const int _eleTSpeed;
    const int _eleAGap;
    const int _stepNum;
    const int _stepPos[];
    
    const int _speIncr = 1;
    int _maxAccSpan;
    int _accSpan
    int _curSpeed;
    int _sarPos;
    int _tarPos;
    int _curPos;
    int _curStep = 0;
    int _dynSpeed;
    int _dynAGap;


    void eleMove(bool reverse) {
      if (reverse) {
        targetSpeed = targetSpeed*(-1);
        speedIncrement = speedIncrement*(-1);
      }

      
      
    }




  public:
    GPElevator(int motorControlPin, int motorPin1, int motorPin2, int targetSpeed, int accelerationGap, int stepNumber, int stepsPosition[]) :
      _eleMCPin(motorControlPin), _eleMPin1(motorPin1), _eleMPin2(motorPin2), _eleTSpeed(targetSpeed), _eleAGap(accelerationGap), _stepNum(stepNumber), _stepPos(stepsPosition)
      {
        Encoder elevator(_eleMPin1, _eleMPin2);
      }

    void Setup() {
      CrcLib::InitializePwmOutput(_eleMCPin);
      _maxAccSpan = _eleTSpeed * _eleAGap;
    }

    void Update() {
      if (isPressed(_eleUBind)) {
        if (_curStep < (_stepNum - 1)) {
          _curStep++;
        }
      } else if (isPressed(_eleDBind)) {
        if (_curStep > 0) {
          _curStep--;
        }
      }

      _tarPos = _stepPos[_curStep];
      _staPos = elevator.read();
    }
};

class Grabber: private Commands {
  private:  
    const int _ser1Pin;
    const int _staPos1 = 0;
    const int _openPos1 = 195;
    const int _cloPos1 = 130;
    const int _ser2Pin;
    const int _staPos2 = 180;
    const int _openPos2 = 5;
    const int _cloPos2 = 60;

    int _serStep = 1;

   public:
    void Setup() {
      CrcLib::InitializePwmOutput(_ser1Pin);
      CrcLib::InitializePwmOutput(_ser2Pin);
    }

    void Update()
      if (isPressed(_graSBind) {
        
      }
};
