#include <CrcLib.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"
#include "binding.h"

using namespace Crc;


//=============================================================================================
//====================================== Indicator ============================================
//=============================================================================================
class Indicator {
  public:
    Indicator(int stripPin, int teamColor) : _strpPin(stripPin), _tmClr(teamColor)
      {
        Adafruit_NeoPixel strip(_ldCnt, stripPin, NEO_GRB + NEO_KHZ800);
        strip.begin();

        yellow = strip.Color(255, 255, 0);
        red = strip.Color(255, 0, 0);
        green = strip.Color(0, 255, 0);
        blue = strip.Color(0, 0, 255);
      }

    void teamColor() {
      if (_tmClr == 0) {
        strip.fill(blue, 0);
      } else if (_tmClr == 1) {
        strip.fill(yellow, 0);
      }
    }

    void working() {
      strip.fill(red, 0);
    }

    void ready() {
      strip.fill(green, 0);
    }

    void color(byte r, byte g, byte b) {
      int clr = strip.Color(r, g, b);
      strip.fill(clr, 0);
    }

  private:
    const int _strpPin;
    const int _tmClr;
    const int _ldCnt = 50;

    Adafruit_NeoPixel strip;

    int yellow;
    int red;
    int green;
    int blue;
};





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
      int flipSpeed;
      int skipSpeed;
    };

    GPGround(config &conf) : _capBind(conf.captureBinding), _lftCptMPin(conf.leftCaptureMotorPin), _rgtCptMPin(conf.rightCaptureMotorPin), _capMSpeed(conf.captureMotorsSpeed), _capMSSpeed(conf.captureMotorsSlowSpeed), _clrSPin(conf.colorSensorPin), _tmClr(conf.teamColor), _capLSPin(conf.captureLaserSensorPin), _fliLSPin(conf.flipperLaserSensorPin), _fliNBind(conf.flipperNeutralBinding), _fliFBind(conf.flipperFlipBinding), _fliSBind(conf.flipperSkipBinding), _fliSrvPin(conf.flipperServoPin), _fliNPos(conf.flipperNeutralPosition), _fliFPos(conf.flipperFlipPosition), _fliSPos(conf.flipperSkipPosition),
      fliTime(1000), ind(CRC_PWM_12, conf.teamColor)
      {
        CrcLib::InitializePwmOutput(_lftCptMPin);
        CrcLib::InitializePwmOutput(_rgtCptMPin);
        CrcLib::InitializePwmOutput(_fliSrvPin, 500, 2500);
        CrcLib::SetPwmOutput(_fliSrvPin, _fliNPos);
      }

    void Update() {
      if (CrcLib::GetAnalogInput(_capLSPin) < _lasLgtVal) {
        _gp = true;
      }

      _capBindPos = CrcLib::ReadAnalogChannel(_capBind);

      if (_capBindPos >= -118 && _capBindPos < 5) {
        _capSpeed = _capMSpeed;
      } else if (_capBindPos >= 5) {
        _capSpeed = _capMSSpeed;
      }
      gpCapture();

      if (_fliSBind.isPressed()) {
        _fliPos = _fliSPos;
      } else if (_fliFBind.isPressed()) {
        _fliPos = _fliFPos;
      } else if (_fliNBind.isPressed()) {
        _fliPos = _fliNPos;
      }
      gpFlip();

      if (_gp) {
        gpColor();
      }
    }

  private:
    Indicator ind;
    // Capture component
    const CrcUtility::ANALOG _capBind;
    const int _lftCptMPin;
    const int _rgtCptMPin;
    const int _clrSPin;
    const int _capLSPin;
    const int _capMSpeed;
    const int _capMSSpeed;
    const int _tmClr;
    const int _blueClrVal = 40;
    const int _yellowClrVal = 110;
   
    // Flipper component
    Binding _fliNBind;
    Binding _fliFBind;
    Binding _fliSBind;
    const int _fliSrvPin;
    const int _fliLSPin;
    const int _fliNPos;
    const int _fliFPos;
    const int _fliSPos;
    const int _lasLgtVal = 200;
    Time fliTime;

    // Dynamic
    int _capBindPos;
    int _capSpeed;
    bool _clrFnd;
    int _clrVal;
    int _gpClr;  // 0 = blue, 1 = yellow
    int _fliPos; // npos, fpos, spos
    bool _fliRdy;
    bool _fliTimeReset;
    bool _gp;


    void gpCapture() {
      CrcLib::SetPwmOutput(_lftCptMPin, _capSpeed);
      CrcLib::SetPwmOutput(_rgtCptMPin, _capSpeed);

      if (!_clrFnd) {
        if (CrcLib::GetAnalogInput(_capLSPin) < _lasLgtVal) {
          ind.working();
        }
      } else if (_fliRdy) {
          if (_fliPos == _fliNPos) {
            CrcLib::SetPwmOutput(_lftCptMPin, 40);
            CrcLib::SetPwmOutput(_rgtCptMPin, 40);
          } else if (_fliPos == _fliSPos) {
            CrcLib::SetPwmOutput(_lftCptMPin, 80);
            CrcLib::SetPwmOutput(_rgtCptMPin, 80);        
          }
          _fliRdy = false;
          _clrFnd = false;
          _gp = false;
      }
    }

    void gpColor() {
      _clrVal = CrcLib::GetAnalogInput(_clrSPin);

      if (_clrVal >= (_blueClrVal - 10) && _clrVal <= (_blueClrVal + 10)) {
        _gpClr = 0;
      } else if (_clrVal >= (_yellowClrVal - 10) && _clrVal <= (_yellowClrVal + 10)) {
        _gpClr = 1;
      } else {
        _gpClr = 2;
      }

      if (_gpClr == _tmClr || _gpClr == 2) {
        _fliPos = _fliSPos;
      } else {
        _fliPos = _fliNPos;
      }

      _clrFnd = true;
      ind.ready();
    }

    void gpFlip() {
      CrcLib::SetPwmOutput(_fliSrvPin, _fliPos);
      if (_gp) {
        if (!_fliTimeReset) {
          fliTime.reset();
          _fliTimeReset = true;
        } else if (fliTime.singleState()) {
          _fliRdy = true;
        }
      }
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

    GPElevator(config &conf) : _offButton(conf.offButton), _s1Button(conf.step1Button), _s2Button(conf.step2Button), _s3Button(conf.step3Button), _s4Button(conf.step4Button), _s5Button(conf.step5Button), _s6Button(conf.step6Button), _s7Button(conf.step7Button), _eleMCPin(conf.motorControlPin), _revSpd(conf.speed),
      spdTime(conf.accelerationGap), elevator(conf.encoderPin1, conf.encoderPin2)
      {
        _stepPos[0] = conf.step1Position;
        _stepPos[1] = conf.step2Position;
        _stepPos[2] = conf.step3Position;
        _stepPos[3] = conf.step4Position;
        _stepPos[4] = conf.step5Position;
        _stepPos[5] = conf.step6Position;
        _stepPos[6] = conf.step7Position;

        CrcLib::InitializePwmOutput(_eleMCPin);
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
    const int _revSpd;
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
    int _step;
    int _elvState;  // 0: Motor off, 1: Stay at position, 2: Moving
    int _curRevSpd;
    int _curSpd;
    int _lstPos;
    int _tarPos;
    int _posInt;
    int _expInt;

    Time spdTime;
    Encoder elevator;


    void setPos() {
      _tarPos = _stepPos[_step];

      if (_tarPos > _curPos) {
        _curRevSpd = _revSpd;
      } else if (_tarPos < _curPos) {
        _curRevSpd = _revSpd*(-1);
      }

      _elvState = 2;
    }

    int elvSpeed() {
      if (_elvState == 0) {
        if (_elvState == 0 && _curPos < _stepPos[1])
        _curSpd = 0;
        return;
      } else if (_elvState == 1) {
        _expInt = 0;
      } else if (_elvState == 2) {
        _expInt = _curRevSpd;
      }

      _posInt = _curPos - _lstPos;
      if (_posInt < (_expInt - 1)) {
        _curSpd += 1;
      } else if (_posInt > (_expInt + 1)) {
        _curSpd -= 1;
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
        _srv1Pos[0] = conf.servo1Position_1;
        _srv1Pos[1] = conf.servo1Position_2;
        _srv1Pos[2] = conf.servo1Position_3;
        _srv2Pos[0] = conf.servo2Position_1;
        _srv2Pos[1] = conf.servo2Position_2;
        _srv2Pos[2] = conf.servo2Position_3;

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
