#include <CrcLib.h>
#pragma once

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

  public:    
    Wheels(CrcUtility::ANALOG throttleBinding, CrcUtility::ANALOG sideBinding, CrcUtility::ANALOG rotationBinding, int frontLeftMCPin, int backLeftMCPin, int frontRightMCPin, int backRightMCPin) : 
      mThroBind(throttleBinding), mSideBind(sideBinding), mRotaBind(rotationBinding), mFLPin(frontLeftMCPin), mFRPin(backLeftMCPin), mBLPin(frontRightMCPin), mBRPin(backRightMCPin) {}

    void Setup() {
      //Initialize CRC Functions
      CrcLib::InitializePwmOutput(mFLPin);
      CrcLib::InitializePwmOutput(mBLPin);
      CrcLib::InitializePwmOutput(mFRPin);
      CrcLib::InitializePwmOutput(mBRPin);
    }

    void Update() {
      // Update MoveHolomonic Function
      CrcLib::MoveHolonomic(mThroBind, mRotaBind, mSideBind, mFLPin, mBLPin, mFRPin, mBRPin);
    }
};