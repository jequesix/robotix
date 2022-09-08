#include <CrcLib.h>
#include <Encoder.h>


using namespace Crc;


Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

int r1;

#define limit CRC_DIG_1

int GLOBALE;

void ramasseur(int POSITION, char V_AVANCER) {


CrcLib::Update();
  if ((motor1Enco.read() < POSITION) ) //Encoder position not yet reached
  {

    CrcLib::SetPwmOutput(CRC_PWM_7, V_AVANCER);

  }
  else if  ((motor1Enco.read() > POSITION) )
  {
    int V_RECULER = -1 * V_AVANCER;
    CrcLib::SetPwmOutput(CRC_PWM_7, V_RECULER);
  }
 else if (POSITION >= POSITION - 50 && POSITION <= POSITION + 50)
  {
    CrcLib::SetPwmOutput(CRC_PWM_7, 0);

    GLOBALE = POSITION;


  }

}


void Calibration_ascenseur() {



  while (1 == 1) {
    if (CrcLib::GetDigitalInput(limit) == LOW)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, -50);
    }

    if (CrcLib::GetDigitalInput(limit) == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 0);
      // delay(100);
      motor1Enco.write(0);
      break;
    }

  }
}





void setup()
{
  CrcLib::Initialize();
  

  Serial.begin(2000000);
  CrcLib::InitializePwmOutput(CRC_PWM_7);
  
  Calibration_ascenseur();
motor1Enco.write(0);
}


void loop()

{

CrcLib::Update();

if(CrcLib::IsCommValid() == HIGH)

{

 Serial.println(motor1Enco.read());
 
r1 = CrcLib::ReadDigitalChannel(BUTTON::R1);
if(motor1Enco.read() <= 10200 && r1 == HIGH)
{
  
  
      CrcLib::SetPwmOutput(CRC_PWM_7, 45);
 
 }

 if(motor1Enco.read() <= 10200 && r1 == LOW)
{

 CrcLib::SetPwmOutput(CRC_PWM_7, 0);
}


if (motor1Enco.read() >= 10200 && r1 == HIGH)
{
CrcLib::SetPwmOutput(CRC_PWM_7, 0);
}

if(CrcLib::ReadDigitalChannel(BUTTON::L1) == HIGH)
{
  CrcLib::SetPwmOutput(CRC_PWM_7, -70);
}

if(CrcLib::GetDigitalInput(limit) == HIGH)
{
  motor1Enco.write(0);
}




 


}





  

}
