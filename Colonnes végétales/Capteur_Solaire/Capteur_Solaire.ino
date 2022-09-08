#include <CrcLib.h>

using namespace Crc;
#define Servo1 CRC_PWM_6

unsigned int Capteur;

void setup()
{
    CrcLib::Initialize();
    CrcLib::InitializePwmOutput(Servo1, 500, 2500);
    CrcLib::SetPwmOutput(Servo1, 0);
    Serial.begin(9600);
    
}

void loop()
{
    CrcLib::Update();
    Capteur = CrcLib::GetAnalogInput(CRC_ANA_2);
    Serial.println(Capteur);
    if(Capteur >= 600)
    {
      CrcLib::SetPwmOutput(Servo1, 127);
    }
    
}
