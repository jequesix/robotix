#include <CrcLib.h>
#include <Encoder.h>

using namespace Crc;



void setup()
{
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_3);
  CrcLib::InitializePwmOutput(CRC_PWM_4);
  CrcLib::InitializePwmOutput(CRC_PWM_5);

  CrcLib::SetPwmOutput(CRC_PWM_1, 0);
  CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  CrcLib::SetPwmOutput(CRC_PWM_3, 0);
  CrcLib::SetPwmOutput(CRC_PWM_4, 0);
  CrcLib::SetPwmOutput(CRC_PWM_5, 0);


  Serial.begin(2000000);




}
void loop()
{
  CrcLib::Update();

  if(CrcLib::IsCommValid() == HIGH)
{

  //mapping manette

  int valY1 = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
  valY1 = map(valY1, -128, 127, -70, 70);

  int valX1 = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
  valX1 = map(valX1, -128, 127, -55, 55);


  int valX2 = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
  valX2 = map(valX2, -128, 127, -35, 35);

  CrcLib::MoveHolonomic(valY1, valX2, valX1, CRC_PWM_1, CRC_PWM_2, CRC_PWM_3, CRC_PWM_4);

  if (CrcLib::ReadDigitalChannel(BUTTON::SELECT) == HIGH)
  
  {
    CrcLib::SetPwmOutput(CRC_PWM_5, 127);
  }

  else if (CrcLib::ReadDigitalChannel(BUTTON::L1) == LOW)
  
  {
    CrcLib::SetPwmOutput(CRC_PWM_5, 0);
  }






}
}
