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
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
bool piton = HIGH;
bool piton2;


#define Moteur_base CRC_PWM_1
#define Moteur_hauteur CRC_PWM_7
#define Moteur_extension CRC_PWM_10
#define ServoPince CRC_PWM_5
#define Bouton_Power CRC_DIG_1

//Scope et fonction encodeur*****************************
Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

bool motorRotating = LOW;

int GLOBALE;

void rotation_encodeur(int POSITION, char V_AVANCER) {

  CrcLib::Update();


  if ((motor1Enco.read() < POSITION) )
  {

    CrcLib::SetPwmOutput(Moteur_base, V_AVANCER);

  }
  else if  ((motor1Enco.read() > POSITION) )
  {
    int V_RECULER = -1 * V_AVANCER;
    CrcLib::SetPwmOutput(Moteur_base, V_RECULER);
  }
  else
  {
    CrcLib::SetPwmOutput(Moteur_base, 0);

    GLOBALE = POSITION;
  }

}

//**************Scope et fonction moteur Banebot extension

void Banebot_extension (const long intervalle2)

{

    CrcLib::SetPwmOutput(Moteur_extension, -127);


 delay(intervalle2);
 piton2 = LOW;

}



// Moteur hauteur fonction

void Banebot_hauteur(const long intervalle)
{

   CrcLib::SetPwmOutput(Moteur_hauteur, -127);


 delay(intervalle);
 piton = LOW;
  }



//Scope et fonction servomoteur pince
unsigned long previousMillis3 = 0;
bool piton3 = LOW;

void Pince(int value, unsigned long interval) {
  unsigned long currentMillis = millis();

  if ((currentMillis - previousMillis3) >= interval) {

    CrcLib::SetPwmOutput(ServoPince, 0);
    piton3 = HIGH;

  }
  else if ((currentMillis - previousMillis3) <= interval) {

    CrcLib::SetPwmOutput(ServoPince, value);

  }

}




void setup() {

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

  motor1Enco.write(0);

  step0 = HIGH; step1 = LOW; step2 = LOW; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
}


void loop()
{

  if (CrcLib::GetDigitalInput(Bouton_Power) == LOW)
  {

    while (1 == 1)
    {
      CrcLib::Update();
      Serial.print(" Valeur encodeur : ");
      Serial.print(motor1Enco.read());
      Serial.println();


      if (step0 == HIGH)
      {
        Banebot_extension(10000);

        if (piton2 == LOW)
        {
          step0 = LOW; step1 = HIGH; step2 = LOW; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
        }

      }

      if (step1 == HIGH)
      {
       Banebot_hauteur(5000);

        if (piton == LOW)
        {

          step0 = LOW; step1 = LOW; step2 = HIGH; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
        }
      }

    }
  }
}
