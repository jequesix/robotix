#include <CrcLib.h>
#include <Encoder.h>
using namespace Crc;

bool step0 = LOW;
bool step1 = LOW;
bool step2 = LOW;
bool step3 = LOW;
bool step4 = LOW;
bool step5 = LOW;
bool step6 = LOW;


#define Motor_control_1 CRC_PWM_1
#define Bouton_Power CRC_DIG_1

Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

bool motorRotating = LOW; // will allow Motor_1 rotation

int GLOBALE;

//**************************Déclaration de la fonction de l'encodeur***************

void rotation_encodeur(int POSITION,char V_AVANCER){

   CrcLib::Update();
    
  if((motor1Enco.read() < POSITION) ) //Encoder position not yet reached
  {
   
   CrcLib::SetPwmOutput(Motor_control_1,V_AVANCER);
  
  }
  else if  ((motor1Enco.read() > POSITION) )
  {
    int V_RECULER = -1*V_AVANCER;
    CrcLib::SetPwmOutput(Motor_control_1,V_RECULER); 
  }
  else 
  { 
   CrcLib::SetPwmOutput(Motor_control_1,0);  
    GLOBALE = POSITION;
 }

}
//***************************************************************************


void setup() {
  CrcLib::Initialize();
  
  Serial.begin(2000000);  
  CrcLib::SetDigitalPinMode(Bouton_Power,INPUT);
  CrcLib::InitializePwmOutput(Motor_control_1); //Use CrcLib::InitializePwmOutput(Motor_control_1,1); to rapidly reverse this motor rotation direction all over the code
  
  motor1Enco.write(0);
 
  step0 = HIGH; step1 = LOW; step2 = LOW; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
}

//***************************************************************************
void loop() {
  
  if (CrcLib::GetDigitalInput(Bouton_Power)==LOW) //The button is pressed
  {
    while(1==1)
    {
      
      CrcLib::Update();    
       Serial.println(motor1Enco.read());
     
      if(step0 == HIGH)
      {

        rotation_encodeur(3500,60);
                  
          if(GLOBALE >= 3500) 
          {
              
              step0 = LOW; step1 = HIGH; step2 = LOW; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
          }
      }
      //------------------
      if(step1 == HIGH)
      {
       
        rotation_encodeur(100,120);
    
        if(GLOBALE >= 100) 
        {
          
          step0 = LOW; step1 = LOW; step2 = HIGH; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
        }
     
}
}
}
}
