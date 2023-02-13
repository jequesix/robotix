#include <CrcLib.h>
using namespace Crc;

int _capMSpeed = 50;
int _capMSSpeed = 30;

void setup() {
  CrcLib::Initialize();

}

void loop() {
  CrcLib::Update();
    if (CrcLib::IsCommValid()) {
      if (CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L) >= -118 && CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L) < 5) {
        CrcLib::SetPwmOutput(CRC_PWM_8, _capMSpeed);
        CrcLib::SetPwmOutput(CRC_PWM_2, _capMSpeed);
      } else if (CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L)) {
        CrcLib::SetPwmOutput(CRC_PWM_8, _capMSSpeed);
        CrcLib::SetPwmOutput(CRC_PWM_2, _capMSSpeed);
      }
    }
}
