#include <CrcLib.h>
#include <Encoder.h>


using namespace Crc;




Encoder motor2Enco(CRC_SERIAL_TXD1, CRC_SERIAL_RXD1);



#define limit2 CRC_DIG_2


void Calibration_trieur () {

  while (1 == 1) {


    if ( CrcLib::GetDigitalInput(limit2) == LOW )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, 20);
    }
    if ( CrcLib::GetDigitalInput(limit2) == HIGH )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, 0);

      motor2Enco.write(0);
      break;

    }
  }
}





void setup() {
    CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_3);
  CrcLib::InitializePwmOutput(CRC_PWM_4);
  CrcLib::InitializePwmOutput(CRC_PWM_5);
  CrcLib::InitializePwmOutput(CRC_PWM_6);
  CrcLib::InitializePwmOutput(CRC_PWM_7);
  CrcLib::InitializePwmOutput(CRC_PWM_8);
  CrcLib::InitializePwmOutput(CRC_PWM_9);
  CrcLib::InitializePwmOutput(CRC_PWM_10);

  /*  pinMode(CRC_SERIAL_TX1, INPUT);
    pinMode(CRC_SERIAL_RX1, INPUT);*/

  Serial.begin(2000000);


  Calibration_trieur();

}

void loop() {
  CrcLib::Update();
  
  Serial.println(motor2Enco.read());
  
  if (CrcLib::IsCommValid() == HIGH)
  {
  
    if(CrcLib::ReadDigitalChannel(BUTTON::START) == HIGH)
    {


CrcLib::SetPwmOutput(CRC_PWM_9, -10);
    }



    
  }


}
