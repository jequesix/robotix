#include <CrcLib.h>

using namespace Crc;

void setup() {
    CrcLib::Initialize();

    CrcLib::InitializePwmOutput(CRC_PWM_1);
    CrcLib::InitializePwmOutput(CRC_PWM_2);
    
   
}
void loop() {
  
    CrcLib::Update();

    CrcLib::MoveTank(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_Y, CRC_PWM_1, CRC_PWM_2);
    
    
}
