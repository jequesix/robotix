#include <CrcLib.h>
#include <Encoder.h>
#include "time.h"
#include "binding.h"


//=============================================================================================
//====================================== GP Elevator ==========================================
//=============================================================================================
class GPElevator {
  public:
    struct config {
      ANALOG manualControl;
      BUTTON resetButton;
      BUTTON offButton;
      int motorControlPin;
      int encoderPin1;
      int encoderPin2;
      int speed;
      int accelerationGap;
    };

    GPElevator(config &conf) : _manCtrl(conf.manualControl), _rstButton(conf.resetButton), _offButton(conf.offButton), _eleMCPin(conf.motorControlPin), _revSpd(conf.speed), _accGap(conf.accelerationGap),
      elevator(conf.encoderPin1, conf.encoderPin2), spdTime(conf.accelerationGap), rvsDclTime(20, (conf.speed + 1))
      {
        CrcLib::InitializePwmOutput(_eleMCPin);
        _elvOn = false;

        _dclTime = _revSpd * _accGap;
      }


    void Update() {
      _curPos = elevator.read();
      
      if (_offButton.wasClicked()) {
        _elvOn = !_elvOn;
      }

      elvMove();
    }

  private:
    const int _eleMCPin;
    const int _revSpd;
    const int _accGap;
    int _dclTime;
    const ANALOG _manCtrl;

    Binding _rstButton;
    Binding _offButton;

    int _curPos;
    bool _elvOn;  // 0: Motor off, 1: Stay at position, 2: Moving
    int _curRevSpd;
    int _step;
    int _curSpd;
    int _spdIncr;
    int _lstPos;
    int _tarPos;
    int _posInt;
    int _expInt;
    bool _rvsDclBln = false;
    int _manCtrlPos;
    int _manCtrlMap;

    Time spdTime;
    Time rvsDclTime;
    Encoder elevator;

    void elvSpeed() {
      if (!_elvOn) {
         _curSpd = 0;
        if (_rstButton.wasClicked()) {
          elevator.write(0);
        }
        return;
      } else if (_elvOn) {
        _manCtrlPos = CrcLib::ReadAnalogChannel(_manCtrl);
        _manCtrlMap = map(_manCtrlPos, -128, 127, _revSpd, _revSpd*(-1));

        if (_manCtrlMap < -1 || _manCtrlMap > -1) {
          _expInt =_manCtrlMap;
        } else {
          _expInt = 0;
        }
      }
      
      _posInt = _curPos - _lstPos;
      if (_posInt <= (_expInt - 1)) {
        _curSpd++;
      } else if (_posInt >= (_expInt + 1)) {
        _curSpd--;
      }

      if (_curSpd > 127) {
        _curSpd = 127;
      } else if (_curSpd < -128) {
        _curSpd = -128;
      }

      _lstPos = _curPos;
    }
    
    void elvMove() {
      if (spdTime.singleState()) {
        elvSpeed();
      }

      CrcLib::SetPwmOutput(_eleMCPin, _curSpd);
    }
};

class RotateArm {
  public:
    struct config {
      byte servo1Pin;
      byte servo2Pin;
      byte rotateMotorPin;
      ANALOG raiseArmButton;
    };

    RotateArm(config &conf) : _servo1Pin(conf.servo1Pin), _servo2Pin(conf.servo2Pin), _rotateMotorPin(conf.rotateMotorPin), _raiseArmButton(conf.raiseArmButton) 
    {
      CrcLib::InitializePwmOutput(_rotateMotorPin);
    }

    void Update() {

    }
  
  private:
    byte _servo1Pin;
    byte _servo2Pin;
    byte _rotateMotorPin;
    ANALOG _raiseArmButton;

    
};