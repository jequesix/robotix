#include <CrcLib.h>
#include <Encoder.h>

using namespace Crc;


bool btnVirtuel = LOW;



unsigned long timeCapture = 0;

#define limit2 CRC_DIG_2

Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);



void Calibration_trieur (int V_AVANCER) {
  
while (1 == 1) {
    Serial.println(motor1Enco.read());
    
    if( CrcLib::GetDigitalInput(limit2) == HIGH )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, V_AVANCER);
    }
    if( CrcLib::GetDigitalInput(limit2) == LOW )
    {
     CrcLib::SetPwmOutput(CRC_PWM_9, 0);
     
     motor1Enco.write(0);
     break;
    
    }
  } 
}

void Position_x (int POSITION, char V_AVANCER, int BUFFER) {

while (1 == 1) {
    
    Serial.println(motor1Enco.read());
    if (motor1Enco.read() < POSITION-BUFFER )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, V_AVANCER);
    }
    if  (motor1Enco.read() > POSITION+BUFFER )
    {
      int V_RECULER = -1 * V_AVANCER;
      CrcLib::SetPwmOutput(CRC_PWM_9, V_RECULER);
    }
    if (motor1Enco.read() < BUFFER+POSITION && motor1Enco.read() > POSITION-BUFFER ){ 
      {
      if  ((motor1Enco.read() > POSITION-BUFFER && motor1Enco.read() < POSITION) )
      {
      CrcLib::SetPwmOutput(CRC_PWM_9, 10);
      }
      if  ((motor1Enco.read() < POSITION+BUFFER && motor1Enco.read() > POSITION) )
      {
      CrcLib::SetPwmOutput(CRC_PWM_9, -10);
      }
      if ( (motor1Enco.read() < POSITION+5 && motor1Enco.read() > POSITION-5) ) 
      {
      CrcLib::SetPwmOutput(CRC_PWM_9, 0);
      break;
      }
     }
  }
  }
}



//**********************************************

void setup() {


  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_9);
  CrcLib::InitializePwmOutput(CRC_PWM_10);
  Serial.begin(2000000);
  Calibration_trieur(10);

}

void loop() {


  CrcLib::Update();
  Serial.println(motor1Enco.read());
  
   
  if (CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP) == HIGH)
  {
    
btnVirtuel = HIGH;

  }
  
  if (btnVirtuel == HIGH)
  {
    CrcLib::SetPwmOutput(CRC_PWM_10, 10);
 }

  else

  {
    timeCapture = millis();
    CrcLib::SetPwmOutput(CRC_PWM_10, 0);
  }

  if (millis() - timeCapture > 725)
  {
    CrcLib::SetPwmOutput(CRC_PWM_10, 0);
    btnVirtuel = LOW;
  }
  

  
  
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT) == HIGH)
  {
  
  Position_x(0, 50, 100);                     
  
  }
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT) == HIGH)
  {
  Position_x(1500, 50, 100);                 
  }
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP) == HIGH)
  {
  Position_x(750, 50, 100);                          
  }


}
