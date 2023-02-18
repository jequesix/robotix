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
    struct config {
      ANALOG captureBinding;
      int leftCaptureMotorPin;
      int rightCaptureMotorPin;
      int captureMotorsSpeed;
      int captureMotorsSlowSpeed;
      int colorSensorPin;
      int teamColor;
      int captureLaserSensorPin;
      BUTTON flipperNeutralBinding;
      BUTTON flipperFlipBinding;
      BUTTON flipperSkipBinding;
      int flipperLaserSensorPin;
      int flipperServoPin;
      int flipperNeutralPosition;
      int flipperFlipPosition;
      int flipperSkipPosition;
      int elevatorLaserSensorPin;
    };

    GPGround(config &conf) : _capBind(conf.captureBinding), _lftCptMPin(conf.leftCaptureMotorPin), _rgtCptMPin(conf.rightCaptureMotorPin), _capMSpeed(conf.captureMotorsSpeed), _capMSSpeed(conf.captureMotorsSlowSpeed), _clrSPin(conf.colorSensorPin), _tmClr(conf.teamColor), _capLSPin(conf.captureLaserSensorPin), _fliLSPin(conf.flipperLaserSensorPin), _fliNBind(conf.flipperNeutralBinding), _fliFBind(conf.flipperFlipBinding), _fliSBind(conf.flipperSkipBinding), _fliSrvPin(conf.flipperServoPin), _fliNPos(conf.flipperNeutralPosition), _fliFPos(conf.flipperFlipPosition), _fliSPos(conf.flipperSkipPosition), _elvLSPin(conf.elevatorLaserSensorPin)
      {
        CrcLib::InitializePwmOutput(_lftCptMPin);
        CrcLib::InitializePwmOutput(_rgtCptMPin);
        CrcLib::InitializePwmOutput(_fliSrvPin, 500, 2500);
        _fliPos = _fliNPos;
        CrcLib::SetPwmOutput(_fliSrvPin, _fliPos);
      }

    void Update() {
      _capBindPos = CrcLib::ReadAnalogChannel(_capBind);

      if (_capBindPos >= -127 && _capBindPos < 127) {
        _capSpeed = _capMSSpeed;
      } else if (_capBindPos >= 127) {
        _capSpeed = _capMSpeed;
      } else {
        _capSpeed = 0;
      }
      gpCapture();

      if (_fliSBind.wasClicked()) {
        _fliPos = _fliSPos;
      } else if (_fliFBind.wasClicked()) {
        _fliPos = _fliFPos;
      } else if (_fliNBind.wasClicked()) {
        _fliPos = _fliNPos;
      }
      gpFlip();
    }

  private:
    // Capture component
    const CrcUtility::ANALOG _capBind;
    const int _lftCptMPin;
    const int _rgtCptMPin;
    const int _clrSPin;
    const int _capLSPin;
    const int _capMSpeed;
    const int _capMSSpeed;
    const int _tmClr;
   
    // Flipper component
    Binding _fliNBind;
    Binding _fliFBind;
    Binding _fliSBind;
    const int _fliSrvPin;
    const int _fliLSPin;
    const int _fliNPos;
    const int _fliFPos;
    const int _fliSPos;

    const int _elvLSPin;

    // Dynamic
    int _capBindPos;
    int _capSpeed;
    int _fliPos; // npos, fpos, spos

    void gpCapture() {
      CrcLib::SetPwmOutput(_lftCptMPin, _capSpeed);
      CrcLib::SetPwmOutput(_rgtCptMPin, _capSpeed);
    }

    void gpFlip() {
      CrcLib::SetPwmOutput(_fliSrvPin, _fliPos);
    }
};





//=============================================================================================
//====================================== GP Elevator ==========================================
//=============================================================================================
class GPElevator {
  public:
    struct config {
      BUTTON offButton;
      BUTTON step1Button;
      BUTTON step2Button;
      BUTTON step3Button;
      BUTTON step4Button;
      BUTTON step5Button;
      BUTTON step6Button;
      BUTTON step7Button;
      int step1Position;
      int step2Position;
      int step3Position;
      int step4Position;
      int step5Position;
      int step6Position;
      int step7Position;
      int motorControlPin;
      int encoderPin1;
      int encoderPin2;
      int speed;
      int accelerationGap;
    };

    GPElevator(config &conf) : _offButton(conf.offButton), _s1Button(conf.step1Button), _s2Button(conf.step2Button), _s3Button(conf.step3Button), _s4Button(conf.step4Button), _s5Button(conf.step5Button), _s6Button(conf.step6Button), _s7Button(conf.step7Button), _eleMCPin(conf.motorControlPin), _revSpd(conf.speed), _accGap(conf.accelerationGap),
      elevator(conf.encoderPin1, conf.encoderPin2), spdTime(conf.accelerationGap), rvsDclTime(20, (conf.speed + 1))
      {
        _stepPos[0] = conf.step1Position;
        _stepPos[1] = conf.step2Position;
        _stepPos[2] = conf.step3Position;
        _stepPos[3] = conf.step4Position;
        _stepPos[4] = conf.step5Position;
        _stepPos[5] = conf.step6Position;
        _stepPos[6] = conf.step7Position;

        CrcLib::InitializePwmOutput(_eleMCPin);
        _elvState = 0;

        _dclTime = _revSpd * _accGap;
      }


    void Update() {
      _curPos = elevator.read();

      if(_offButton.wasClicked()) {
        _step = 0;
        setPos();
      } else if (_s1Button.wasClicked()) {
        _step = 1;
        setPos();
      } else if (_s2Button.wasClicked()) {
        _step = 2;
        setPos();
      } else if (_s3Button.wasClicked()) {
        _step = 3;
        setPos();
      } else if (_s4Button.wasClicked()) {
        _step = 4;
        setPos();
      } else if (_s5Button.wasClicked()) {
        _step = 5;
        setPos();
      } else if (_s6Button.wasClicked()) {
        _step = 6;
        setPos();
      } else if (_s7Button.wasClicked()) {
        _step = 7;
        setPos();
      }

      elvMove();
    }

  private:
    const int _eleMCPin;
    const int _revSpd;
    const int _accGap;
    int _dclTime;
    int _stepPos[7];
    Binding _offButton;
    Binding _s1Button;
    Binding _s2Button;
    Binding _s3Button;
    Binding _s4Button;
    Binding _s5Button;
    Binding _s6Button;
    Binding _s7Button;

    int _curPos;
    int _elvState;  // 0: Motor off, 1: Stay at position, 2: Moving
    int _curRevSpd;
    int _step;
    int _curSpd;
    int _spdIncr;
    int _lstPos;
    int _tarPos;
    int _posInt;
    int _expInt;
    bool _rvsDclBln = false;

    Time spdTime;
    Time rvsDclTime;
    Encoder elevator;


    void setPos() {
      if (_step == 0) {
        _elvState = 0;
        return;
      } else {
        _tarPos = _stepPos[_step - 1];
        _elvState = 2;

        if (_tarPos >= _curPos) {
          _curRevSpd = _revSpd;
          _spdIncr = 1;
        } else if (_tarPos < _curPos) {
          _curRevSpd = _revSpd*(-1);
          _spdIncr = -1;
        }
      }
    }

    void elvSpeed() {
      if (_elvState == 0) {
         _curSpd = 0;
        return;
      } else if (_elvState == 1) {
        _expInt = 0;
      } else if (_elvState == 2) {
        if (abs(_tarPos - _curPos) <= (_dclTime * 2)) {
          _expInt -= _spdIncr;
        } else {
          _expInt = _curRevSpd;
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
      if (_elvState != 0) {
        if (_curPos <= (_tarPos + 1000) && _curPos >= _tarPos) {
          _elvState = 1;
        } else {
          setPos();
        }
      }

      if (_spdIncr == -1 && !_rvsDclBln) {
        if (rvsDclTime.cycleState() < (_revSpd + 1)) {
          elvSpeed();
        } else {
          _rvsDclBln = true;
        }
      } else if (spdTime.singleState()) {
        elvSpeed();
      }
      Serial.println(_curSpd);
      CrcLib::SetPwmOutput(_eleMCPin, _curSpd);
    }
};





//=============================================================================================
//====================================== GP Grabber ===========================================
//=============================================================================================
class GPGrabber {
  public:
    struct config {
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

    GPGrabber(config &conf) : _graSrvButton(conf.servoBinding), _graWhlButton(conf.wheelBinding), _srv1Pin(conf.servo1Pin), _srv2Pin(conf.servo2Pin), _whlPin(conf.wheelPin), _whlSpeed(conf.wheelSpeed)
      {
        _srv1Pos[0] = conf.servo1Position_2;
        _srv1Pos[1] = conf.servo1Position_3;
        _srv2Pos[0] = conf.servo2Position_2;
        _srv2Pos[1] = conf.servo2Position_3;

        CrcLib::InitializePwmOutput(_srv1Pin, 500, 2500);
        CrcLib::InitializePwmOutput(_srv2Pin, 500, 2500);
        CrcLib::InitializePwmOutput(_whlPin);
        
        CrcLib::SetPwmOutput(_srv1Pin, conf.servo1Position_1);
        CrcLib::SetPwmOutput(_srv2Pin, conf.servo2Position_1);
        
      }


    void Update() {
      if (_graSrvButton.wasClicked()) {
        if (_step < (_stepNum - 1)) {
          _step++;
        } else {
          _step = 0;
        }
        srvMove();
      }



      if (_graWhlButton.isPressed()) {
        whlSpin(_whlSpeed);
      } else {
        whlSpin(0);
      }
    }

  private:
    Binding _graWhlButton;
    const int _srv1Pin;
    int _srv1Pos[2];
    const int _srv2Pin;
    int _srv2Pos[2];
    const int _whlPin;
    const int _whlSpeed;
    Binding _graSrvButton;
    
    const int _stepNum = 2;
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
