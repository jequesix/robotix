#pragma once
#include <CrcLib.h>
using namespace Crc;

class Wheels {
  private:
    const int mFLPin;
    const int mBLPin;
    const int mFRPin;
    const int mBRPin;

    const CrcUtility::ANALOG mThroBind;
    const CrcUtility::ANALOG mSideBind;
    const CrcUtility::ANALOG mRotaBind;

    int _throBindPos;
    int _throBindMap;
    int _sideBindPos;
    int _sideBindMap;
    int _rotaBindPos;
    int _rotaBindMap;

    int  _speed;
    int _rvsSpeed;

    int _trnSpeed;

  public:    
    Wheels(CrcUtility::ANALOG throttleBinding, CrcUtility::ANALOG sideBinding, CrcUtility::ANALOG rotationBinding, int frontLeftMCPin, int backLeftMCPin, int frontRightMCPin, int backRightMCPin, int wheelSpeed) : 
      mThroBind(throttleBinding), mSideBind(sideBinding), mRotaBind(rotationBinding), mFLPin(frontLeftMCPin), mFRPin(backLeftMCPin), mBLPin(frontRightMCPin), mBRPin(backRightMCPin), _speed(wheelSpeed) {}

    void Setup() {
      //Initialize CRC Functions
      CrcLib::InitializePwmOutput(mFLPin);
      CrcLib::InitializePwmOutput(mBLPin);
      CrcLib::InitializePwmOutput(mFRPin);
      CrcLib::InitializePwmOutput(mBRPin);

      _rvsSpeed = _speed*(-1);
    }

    void Update() {
      _throBindPos = CrcLib::ReadAnalogChannel(mThroBind);
      _sideBindPos = CrcLib::ReadAnalogChannel(mSideBind);
      _rotaBindPos = CrcLib::ReadAnalogChannel(mRotaBind);

      if (_rotaBindPos > 5 && _rotaBindPos < -5) {
        _speed *= 2;
        _rvsSpeed *= 2;
      }
      
      _throBindMap = map(_throBindPos, -128, 127, _rvsSpeed, _speed);
      _sideBindMap = map(_sideBindPos, -128, 127, _rvsSpeed, _speed);
      _rotaBindMap = map(_rotaBindPos, -128, 127, _rvsSpeed, _speed);
    
      // Update MoveHolomonic Function
      CrcLib::MoveHolonomic(_throBindMap, _rotaBindMap, _sideBindMap, mFLPin, mBLPin, mFRPin, mBRPin);
    }
};
