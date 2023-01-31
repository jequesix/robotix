#include <CrcLib.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"

using namespace Crc;


class Commands {
  private:
    bool _pressed = false;

    bool isPressed(button) {
      if (CrcLib::ReadDigitalChannel(button) == 0) {
        if (_pressed == false) {
          return false;
        } else {
          _pressed = false;
          return false;
        }        
      } else if (CrcLib::ReadDigitalChannel(button) == 1) {
         if (_pressed == true) {
          return false;
         } else {
          _pressed = true;
          return true;
         }
};

class GPFlip: private Commands {
  private:
    // 0 = auto, 1 = manual
    int _mode = 0;
    const CrcUtility::BUTTON _modeBind;

    // Capture component
    const int _lasSPin;
    bool _isGp;

    const int _entMPin;
    const int _forMPin;

    const CrcUtility::BUTTON _capBind;
    const int _capMSpeed;
    
    // Flipper component
    // 0 = blue, 1 = yellow
    const int _tColor = 0;
    const int _blue = 500;
    const int _yellow = 200;
    const int _colInterval = 10;
    int _colSPin;
    const int _colSLPin;
    // Number of leds - 1
    int _colSLNum = 4;
    int _colValues[2];
    bool _colScanned = false;


    const int _fliMCPin;
    const int _fliMPin1;
    const int _fliMPin2;

    const int _fliMSpeed;
    const int _fliSMSpeed;
    const int _fliFPos;
    const int _fliSPos;
    const int _fliInterval = 10;


    void gpCapture(int motorSpeed) {
      CrcLib::SetPwmOutput(_entMPin, motorSpeed);
      CrcLib::SetPwmOutput(_forMPin, motorSpeed);
    }

    void gpFindColor() {
      if (_colScanned == false) {
        int cSState = CrcLib::GetAnalogInput(_colSPin);

        switch(gpTime.cycleState(100, 3))
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
      }
    }

    void gpFlip(int pos) {
      if (flipper.read() < pos - _fliInterval) {
        CrcLib::SetPwmOutput(_fliMCPin, _fliMSpeed);
      } else if (flipper.read() > pos + _fliInterval) {
        CrcLib::SetPwmOutput(_fliMCPin, _fliSMSpeed);
      } else if (flipper.read() >= pos - _fliInterval && flipper.read() <= pos + _fliInterval) {
        CrcLib::SetPwmOutput(_fliMCPin, 0);
      }
    }

  public:
    GPFlip(int teamColor, CrcUtility::BUTTON modeBinding, CrcUtility::BUTTON captureBinding, int laserSensorPin, int colorSensorPin,, int colorSensorLedPin, int entryMotorPin, int forwardMotorPin, int captureMotorsSpeed, int flipperMotorControlPin, int flipperMotorPin1, int flipperMotorPin2, int flipperMotorSpeed) : 
      _tColor(teamColor), _modeBind(modeBinding), _capBind(captureBinding), _lasSPin(laserSensorPin), _colSPin(colorSensorPin), _colSLPin(colorSensorLedPin) _entMPin(entryMotorPin), _forMPin(forwardMotorPin), _capMSpeed(captureMotorsSpeed), _fliMCPin(flipperMotorControlPin), _fliMPin1(flipperMotorPin1), _fliMPin2(flipperMotorPin2), _fliMSpeed(flipperMotorSpeed) {
        Time gpTime();
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
      CrcLib::InitializePwmOutput(_forMPin);
    }

    void Update() {
      lasSState = CrcLib::GetDigitalInput(_lasSPin);
      if (lasSState == 0) {
        _isGp = true;
      }

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
            int gpColor = gpFindColor();
            gpCapture(_capMSpeed);
            break;
          case 2:
            gpCapture(0);

            if (gpColor == 2 || _tColor == gpColor) {
              gpFlip(_fliSPos, _fliMSpeed);
            } else if (_tColor != gpColor) {
              gpFlip(_fliFPos);
            }
          case 3:
            gpFlip(0);
      }
    }
};

class GPElevator: private Commands {
  private:
    const int _eleMCPin;
    const int _eleMPin1;
    const int _eleMPin2;
    
};
