#include <CrcLib.h>
using namespace Crc;

class Capture {
  private:
    const int mRecMPin;
    const CrcUtility::BUTTON mRecMBind;

  public:
    Capture(int receiverMotorPin, CrcUtility::BUTTON receiverMotorBinding) : mRecMPin(receiverMotorPin), mRecMBind(receiverMotorBinding) {}

    void Setup() {
      //Initialize Crc Functions
      CrcLib::InitializePwmOutput(mRecMPin);
    }

    void Update() {
      if (CrcLib::ReadDigitalChannel(mRecMBind) == 1) {
        CrcLib::SetPwmOutput(mRecMPin, 50);
      }
    }
};
