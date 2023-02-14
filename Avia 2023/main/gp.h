#include <CrcLib.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"
#include "binding.h"

using namespace Crc;





//=============================================================================================
//====================================== GP Ground ============================================
//=============================================================================================
class GPGround {
  public:
    GPGround(ANALOG captureBinding, ANALOG captureReverseBinding, int leftCaptureMotorPin, int rightCaptureMotorPin, int captureMotorsSpeed, int captureMotorsSlowSpeed, 
      BUTTON flipperNeutralBinding, BUTTON flipperFlipBinding, BUTTON flipperSkipBinding, int flipperServoPin, int flipperNeutralPosition, int flipperFlipPosition, int flipperSkipPosition) : 
      _capBind(captureBinding), _capRBind(captureReverseBinding), _lftCptMPin(leftCaptureMotorPin), _rgtCptMPin(rightCaptureMotorPin), _capMSpeed(captureMotorsSpeed), _capMSSpeed(captureMotorsSlowSpeed), 
      _fliNBind(flipperNeutralBinding), _fliFBind(flipperFlipBinding), _fliSBind(flipperSkipBinding), _fliSrvPin(flipperServoPin), _fliNPos(flipperNeutralPosition), _fliFPos(flipperFlipPosition), _fliSPos(flipperSkipPosition) {}


    void Setup() {
      CrcLib::InitializePwmOutput(_lftCptMPin);
      CrcLib::InitializePwmOutput(_rgtCptMPin);
      CrcLib::InitializePwmOutput(_fliSrvPin, 500, 2500);
      CrcLib::SetPwmOutput(_fliSrvPin, _fliNPos);
    }

    void Update() {
      _capBindPos = CrcLib::ReadAnalogChannel(_capBind);

      // Manual
      if (_capBindPos >= -118 && _capBindPos < 5) {
        gpCapture(_capMSpeed);
      } else if (_capBindPos >= 5) {
        gpCapture(_capMSSpeed);
      } else if (CrcLib::ReadAnalogChannel(_capRBind) >= -118) {
        gpCapture(_capMSSpeed*(-1));
      }

      if (_fliSBind.isPressed()) {
        gpFlip(_fliSPos);
      } else if (_fliFBind.isPressed()) {
        gpFlip(_fliFPos);
      } else if (_fliNBind.isPressed()) {
        gpFlip(_fliNPos);
      }
    }

  private:
    // Capture component
    const CrcUtility::ANALOG _capBind;
    const CrcUtility::ANALOG _capRBind;
    const int _lftCptMPin;
    const int _rgtCptMPin;
    const int _capMSpeed;
    const int _capMSSpeed;
    
    // Flipper component
    Binding _fliNBind;
    Binding _fliFBind;
    Binding _fliSBind;
    const int _fliSrvPin;
    const int _fliNPos;
    const int _fliFPos;
    const int _fliSPos;

    // Dynamic
    int _capBindPos;


    void gpCapture(int motorSpeed) {
      CrcLib::SetPwmOutput(_lftCptMPin, motorSpeed);
      CrcLib::SetPwmOutput(_rgtCptMPin, motorSpeed);
    }

    void gpFlip(int pos) {
      CrcLib::SetPwmOutput(_fliSrvPin, pos);
    }
};





//=============================================================================================
//====================================== GP Elevator ==========================================
//=============================================================================================
class GPElevator {
  public:
    GPElevator(BUTTON offButton, BUTTON step1Button, BUTTON step2Button, BUTTON step3Button, BUTTON step4Button, BUTTON step5Button, BUTTON step6Button, BUTTON step7Button, int motorControlPin, uint8_t motorPin1, uint8_t motorPin2, int targetSpeed, int stepNumber,
    int step1Position, int step2Position, int step3Position, int step4Position, int step5Position, int step6Position, int step7Position) :
    _offButton(offButton), _s1Button(step1Button), _s2Button(step2Button), _s3Button(step3Button), _s4Button(step4Button), _s5Button(step5Button), _s6Button(step6Button), _s7Button(step7Button), _eleMCPin(motorControlPin), _eleMPin1(motorPin1), _eleMPin2(motorPin2), _eleTSpeed(targetSpeed), _stepNum(stepNumber),
    spdTime(10), elevator(_eleMPin1, _eleMPin2)
      {
        _stepPos = new int[_stepNum];
        _stepPos[0] = step1Position;
        _stepPos[1] = step2Position;
        _stepPos[2] = step3Position;
        _stepPos[3] = step4Position;
        _stepPos[4] = step5Position;
        _stepPos[5] = step6Position;
        _stepPos[6] = step7Position;
      }

    void Setup() {
      CrcLib::InitializePwmOutput(_eleMCPin);
      _plsPrChk = 15;
    }

    void Update() {
      _curPos = elevator.read();

      if(_offButton.wasClicked()) {
        _elvState = 0;
      } else if (_s1Button.wasClicked()) {
        _step = 0;
        setPos();
      } else if (_s2Button.wasClicked()) {
        _step = 1;
        setPos();
      } else if (_s3Button.wasClicked()) {
        _step = 2;
        setPos();
      } else if (_s4Button.wasClicked()) {
        _step = 3;
        setPos();
      } else if (_s5Button.wasClicked()) {
        _step = 4;
        setPos();
      } else if (_s6Button.wasClicked()) {
        _step = 5;
        setPos();
      } else if (_s7Button.wasClicked()) {
        _step = 6;
        setPos();
      }

      elvMove();
    }

  private:
    const int _eleMCPin;
    const uint8_t _eleMPin1;
    const uint8_t _eleMPin2;
    const int _eleTSpeed;
    const int _spdCnst = 2;
    const int _stepNum;
    int *_stepPos;
    Binding _offButton;
    Binding _s1Button;
    Binding _s2Button;
    Binding _s3Button;
    Binding _s4Button;
    Binding _s5Button;
    Binding _s6Button;
    Binding _s7Button;

    double _plsPrChk;

    int _curPlsPrChk;
    int _curTgtSpeed;
    int _curSpeed;
    int _staPos;
    int _lstPos;
    int _tarPos;
    int _posInt;
    int _expInt;
    int _curPos;
    int _step;
    int _elvState;  // 0: Motor off, 1: Stay at position, 2: Moving

    Time spdTime;
    Encoder elevator;


    void setPos() {
      _tarPos = _stepPos[_step];
      _staPos = _curPos;

      if (_tarPos > _staPos) {
        _curTgtSpeed = _eleTSpeed;
        _curPlsPrChk = _plsPrChk;
      } else if (_tarPos < _staPos) {
        _curTgtSpeed = _eleTSpeed*(-1);
        _curPlsPrChk = _plsPrChk*(-1);
      }

      _elvState = 2;
    }

    int elvSpeed() {
      if (_elvState == 0) {
        if (_elvState == 0 && _curPos < _stepPos[1])
        _curSpeed = 0;
        return;
      } else if (_elvState == 1) {
        _expInt = 0;
      } else if (_elvState == 2) {
        _expInt = _curPlsPrChk;
      }

      _posInt = _curPos - _lstPos;
      if (_posInt < (_expInt - 1)) {
        _curSpeed += 1;
      } else if (_posInt > (_expInt + 1)) {
        _curSpeed -= 1;
      }

      _lstPos = _curPos;
    }
    
    void elvMove() {
      if (_curPos <= (_tarPos + 5) && _curPos >= (_tarPos - 5)) {
        _elvState = 1;
      }

      if (spdTime.singleState()) {
        elvSpeed();
      }

      CrcLib::SetPwmOutput(_eleMCPin, _curSpeed);
    }
};





//=============================================================================================
//====================================== GP Grabber ===========================================
//=============================================================================================
class GPGrabber {
  public:
    struct config{
      CrcUtility::BUTTON servoBinding; 
      CrcUtility::BUTTON wheelBinding; 
      int servo1Pin; 
      int servo2Pin; 
      int servo1Position_1;
      int servo1Position_2;
      int servo1Position_3;
      int servo2Position_1;
      int servo2Position_2;
      int servo2Position_3;
      int wheelPin; 
      int wheelSpeed;
    };

    GPGrabber(GPGrabber::config &conf) :
      _graSrvButton(conf.servoBinding), _graWhlButton(conf.wheelBinding), _srv1Pin(conf.servo1Pin), _srv2Pin(conf.servo2Pin), _whlPin(conf.wheelPin), _whlSpeed(conf.wheelSpeed)
      {
        _srv1Pos[0] = conf.servo1Position_1;
        _srv1Pos[1] = conf.servo1Position_2;
        _srv1Pos[2] = conf.servo1Position_3;
        _srv2Pos[0] = conf.servo2Position_1;
        _srv2Pos[1] = conf.servo2Position_2;
        _srv2Pos[2] = conf.servo2Position_3;
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
      if (_graWhlButton.isPressed()) {
        whlSpin(_whlSpeed);
      } else {
        whlSpin(0);
      }
    }

  private:
    Binding _graSrvButton;
    Binding _graWhlButton;
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
      CrcLib::SetPwmOutput(_srv1Pin, _srv1Pos[_step]);
      CrcLib::SetPwmOutput(_srv2Pin, _srv2Pos[_step]);
    }

    void whlSpin(int whlSpeed, bool reverse = false) {
      if (reverse) {
        whlSpeed = whlSpeed*(-1);
      }

      CrcLib::SetPwmOutput(_whlPin, whlSpeed);
    }
};
