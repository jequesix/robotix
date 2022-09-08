#include <CrcLib.h>
using namespace Crc;
#define Motor_control_2 CRC_PWM_2
unsigned long previousMillis = 0;

void Banebot(const long interval){
   unsigned long currentMillis = millis();
 // CrcLib::SetPwmOutput(Msotor_control_2, 30);
  
  
  if ((currentMillis - previousMillis) >= interval) {
      
           CrcLib::SetPwmOutput(Motor_control_2, 0);
      
    
  }
  else if ((currentMillis - previousMillis) <= interval){
    
    CrcLib::SetPwmOutput(Motor_control_2, -10);
  }
  
}



void setup() {
  CrcLib::Initialize();
  CrcLib::Update();
  CrcLib::InitializePwmOutput(Motor_control_2);
Serial.begin(2000000);
  

}

void loop() {
 
Banebot(2000);



}
