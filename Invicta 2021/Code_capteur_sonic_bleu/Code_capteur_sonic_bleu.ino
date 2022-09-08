#include <SR04.h>
#include <CrcLib.h>

using namespace Crc;
#define Bouton_Power CRC_DIG_1
#define TRIG_PIN CRC_DIG_6
#define ECHO_PIN CRC_PWM_6

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

//***************Fonction Capteur Ultrasonic Bleu***************

void Capteur_sonic_bleu()
{
  
  Serial.println("Capteur 2 : ");
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
}

void setup() 
{
  CrcLib::Initialize();
    CrcLib::InitializePwmOutput(ECHO_PIN);
   CrcLib::SetDigitalPinMode(TRIG_PIN, INPUT);
 Serial.begin(2000000); 
 delay(100);
 }

void loop() {
  
  if(CrcLib::GetDigitalInput(Bouton_Power)== LOW)
{
  while(1==1)
{
   
Capteur_sonic_bleu();

}
}
}
