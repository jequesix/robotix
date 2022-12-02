#include <CrcLib.h>

using namespace Crc;

void setup() {
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1); //Initialiser les sorties pwm moteur
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_3);
  CrcLib::InitializePwmOutput(CRC_PWM_4);

  CrcLib::SetPwmOutput(CRC_PWM_1, 0);//Mettre tous les moteurs à 0 au début du programme
  CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  CrcLib::SetPwmOutput(CRC_PWM_3, 0);
  CrcLib::SetPwmOutput(CRC_PWM_4, 0);
  CrcLib::SetPwmOutput(CRC_PWM_5, 0);





}



void loop() {
  CrcLib::Update();

  if (CrcLib::IsCommValid() == HIGH)//On vérifie que la manette est bien connectée pour entamer la séquence d'actions

  {

    CrcLib::MoveHolonomic(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_X, ANALOG::JOYSTICK1_X, CRC_PWM_1, CRC_PWM_2, CRC_PWM_3, CRC_PWM_4); //Fonction du move holonomic

    /*On peut "mapper" les valeurs si les moteurs vont trop vite en appuyant juste un peu/les joysticks sont trop "sensibles" :

      int Valeur_Joystick_1Y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y); // mapping pour avancer-reculer : on peut changer les 2 dernières valeurs de la fonction pour ajuster la sensibilité
      Valeur_Joystick_1Y = map(Valeur_Joystick_1Y, -128, 127, -70, 70);


      int Valeur_Joystick_2X = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);// mapping pour mouvement latéral : on peut changer les 2 dernières valeurs de la fonction pour ajuster la sensibilité
      Valeur_Joystick_2X = map(Valeur_Joystick_2X, -128, 127,-55, 55)


      int Valeur_Joystick_1X = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);// mapping pour tourner gauche-droite : on peut changer les 2 dernières valeurs de la fonction pour ajuster la sensibilité
      Valeur_Joystick_1X = map(Valeur_Joystick_1X, -128, 127, -35, 35);

      //On utilisra alors cette fornction holonomic suivante :

      CrcLib::MoveHolonomic(Valeur_Joystick_1Y, Valeur_Joystick_2X, Valeur_Joystick_1X, CRC_PWM_1, CRC_PWM_2, CRC_PWM_3, CRC_PWM_4);
      
      UTILISER LES MOTOR CONTROL IDENTIFIÉS AVEC LES NUMÉROS. LES NUMÉROS CORRESPONDENT AUX NUMÉROS DE PORTS PWM
      
      */

  }


}
