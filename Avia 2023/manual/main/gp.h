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
    // Capture component
    const int _entMPin;
    const CrcUtility::ANALOG _capBind;
    const CrcUtility::ANALOG _capRBind;
    const int _capMSpeed;
    const int _capMSSpeed;
    
    // Flipper component
    const int _fliMCPin;
    const int _fliMSpeed;
    const int _fliSMSpeed;
    const int _fliNPos;
    const int _fliFPos;
    const int _fliSPos;
    int _fliState;
    const CrcUtility::BUTTON _fliNBind;
    const CrcUtility::BUTTON _fliFBind;
    const CrcUtility::BUTTON _fliSBind;

    void gpCapture(int motorSpeed) {
      CrcLib::SetPwmOutput(_entMPin, motorSpeed);
    }

    void gpFlip(int pos) {
      CrcLib::SetPwmOutput(_fliMCPin, pos);
      }





  public:
    GPGround(ANALOG captureBinding, ANALOG reverseCaptureBinding, int entryMotorPin, int captureMotorsSpeed, int captureMotorsSlowSpeed, int flipperMotorPin) : 
      _capBind(captureBinding), _capRBind(reverseCaptureBinding), _entMPin(entryMotorPin), _capMSpeed(captureMotorsSpeed), _capMSSpeed(captureMotorsSlowSpeed), _fliMCPin(flipperMotorPin)
      {
        _fliState = 0;
      }

    void Setup() {
      CrcLib::InitializePwmOutput(_entMPin);
    }

    void Update() {
      // Manual
      if (CrcLib::ReadAnalogChannel(_capBind) >= -118 && CrcLib::ReadAnalogChannel(_capBind) < 0) {
        gpCapture(_capMSpeed);
      } else if (CrcLib::ReadAnalogChannel(_capBind) >= 0) {
        gpCapture(_capMSSpeed);
      } else if (CrcLib::ReadAnalogChannel(_capRBind) >= -118) {
        gpCapture(_capMSSpeed*(-1));
      }

      if (CrcLib::ReadDigitalChannel(_fliFBind) == 1) {
        _fliState = 1;
      } else if (CrcLib::ReadDigitalChannel(_fliSBind) == 1) {
        _fliState = 2;
      } else if (CrcLib::ReadDigitalChannel(_fliNBind) == 1) {
        _fliState = 3;
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
    const uint8_t _eleMPin1;
    const uint8_t _eleMPin2;
    const int _eleTSpeed;
    const int _eleAGap;
    const int _stepNum;
    int *_stepPos;
    CommandButton _s1Button;
    CommandButton _s2Button;
    CommandButton _s3Button;
    CommandButton _s4Button;
    CommandButton _s5Button;
    CommandButton _s6Button;
    CommandButton _s7Button;
    
    const int _spdIncr = 1;

    int _curSpdIncr;
    int _curTgtSpeed;
    int _curSpeed;
    int _staPos;
    int _tarPos;
    int _curPos;
    int _dynStep = 0;

    Encoder elevator;

    void elvMove(bool reverse = false) {
      if (reverse) {
        _curTgtSpeed = _eleTSpeed*(-1);
        _curSpdIncr = _spdIncr*(-1);
      }
      if (1==1) {}
    }




  public:
    GPElevator(BUTTON step1Button, BUTTON step2Button, BUTTON step3Button, BUTTON step4Button, BUTTON step5Button, BUTTON step6Button, BUTTON step7Button, int motorControlPin, uint8_t motorPin1, uint8_t motorPin2, int targetSpeed, int accelerationGap, int stepNumber, int stepsPosition[]) :
    _s1Button(step1Button), _s2Button(step2Button), _s3Button(step3Button), _s4Button(step4Button), _s5Button(step5Button), _s6Button(step6Button), _s7Button(step7Button), _eleMCPin(motorControlPin), _eleMPin1(motorPin1), _eleMPin2(motorPin2), _eleTSpeed(targetSpeed), _eleAGap(accelerationGap), _stepNum(stepNumber),
    elevator(_eleMPin1, _eleMPin2)
      {
        _stepPos = new int[_stepNum];
        for (int i = 0; i < _stepNum ; i++)
        {
          _stepPos[i] = stepsPosition[i];
        }
      }

    void Setup() {
      CrcLib::InitializePwmOutput(_eleMCPin);

    }

    void Update() {
      if(step1Button.wasClicked()) {
        _dynStep = 0;
      } else if (step2Button.wasClicked()) {
        _dynStep = 1;
      } else if (step3Button.wasClicked()) {
        _dynStep = 2;
      } else if (step4Button.wasClicked()) {
        _dynStep = 3;
      } else if (step5Button.wasClicked()) {
        _dynStep = 4;
      } else if (step6Button.wasClicked()) {
        _dynStep = 5;
      } else if (step7Button.wasClicked()) {
        _dynStep = 6;

      _tarPos = _stepPos[_dynStep];
      _staPos = elevator.read();

      if  
        elvMove();


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
