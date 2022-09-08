
//Scope entier*************************
#include <CrcLib.h>
#include <Encoder.h>
#include <Servo.h>

using namespace Crc;

bool step0 = LOW;
bool step1 = LOW;
bool step2 = LOW;
bool step3 = LOW;
bool step4 = LOW;
bool step5 = LOW;
bool step6 = LOW;
bool step7 = LOW;


#define Moteur_base CRC_PWM_1
#define Moteur_hauteur CRC_PWM_8
#define Moteur_extension CRC_PWM_3
#define ServoPince CRC_PWM_5
#define Bouton_Power CRC_DIG_1


void setup() {
  // put your setup code here, to run once:
  CrcLib::Initialize();
  CrcLib::Update();
  Serial.begin(2000000);
  CrcLib::SetDigitalPinMode(Bouton_Power, INPUT);
  CrcLib::InitializePwmOutput(Moteur_base);
  CrcLib::InitializePwmOutput(Moteur_hauteur);
  CrcLib::InitializePwmOutput(Moteur_extension);
  CrcLib::InitializePwmOutput(ServoPince);
  CrcLib::SetPwmOutput(Moteur_base, 0);
  CrcLib::SetPwmOutput(Moteur_hauteur, 0);
  CrcLib::SetPwmOutput(Moteur_extension, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
 CrcLib::SetPwmOutput(Moteur_hauteur,-255);
 delay(2000);
}
