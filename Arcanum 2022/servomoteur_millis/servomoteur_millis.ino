#include <CrcLib.h>



#define servo1 CRC_PWM_6

bool btnAr;

bool btnVirtuel = LOW;

unsigned long timeCapture = 0;

using namespace Crc;


//Fontion sevomoteur avec millis time

void Servo_millis(const unsigned long duree)
{
  btnAr = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
  if (btnAr == HIGH)
  {
    btnVirtuel = HIGH;
  }
  if (btnVirtuel == HIGH)
  {
    CrcLib::SetPwmOutput(servo1, 10);
 }

  else

  {
    timeCapture = millis();
    CrcLib::SetPwmOutput(servo1, 0);
  }

  if (millis() - timeCapture > duree)
  {
    CrcLib::SetPwmOutput(servo1, 0);
    btnVirtuel = LOW;
  }

}

void setup() 
{
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(servo1, 500, 2500);
}

void loop() 
{
  CrcLib::Update();

  Servo_millis(372.25);





}
