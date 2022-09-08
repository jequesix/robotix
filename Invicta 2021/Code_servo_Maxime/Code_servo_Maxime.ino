#include <CrcLib.h>
using namespace Crc;

void setup() 
{
    CrcLib::Initialize();
    CrcLib::InitializePwmOutput(CRC_PWM_5);  
}
void loop() 
{
   CrcLib::Update();
    
   CrcLib::SetPwmOutput(CRC_PWM_5,30);
   delay(500);
  
  CrcLib::SetPwmOutput(CRC_PWM_5,-30 );
  delay(500);

  CrcLib::SetPwmOutput(CRC_PWM_5,60);
  delay(500);
  
  CrcLib::SetPwmOutput(CRC_PWM_5,-60 );
  delay(500);
  
  CrcLib::SetPwmOutput(CRC_PWM_5,90);
  delay(500);
  
  CrcLib::SetPwmOutput(CRC_PWM_5,-90 );
  delay(500);
  
  CrcLib::SetPwmOutput(CRC_PWM_5,120);
  delay(500);
  
  CrcLib::SetPwmOutput(CRC_PWM_5,-120 );
  delay(500);
}
