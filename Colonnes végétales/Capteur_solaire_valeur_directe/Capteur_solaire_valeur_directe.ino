#include <CrcLib.h>
#include <Servo.h>          // Appeler la librairie nécessaire pour les servomoteurs
using namespace Crc;

//servomoteur//

Servo horizontal;
int servoh = 90;            //position du servo horizontal
int servohlimh = 170;      //limite du servo horizontal mouvement horaire
int servohlimah = 10;       //limite du servo horizontal mouvement antihoraire
int servonh = 30;
int posh ;

Servo vertical;
int servov = 90;            //position du servo vertical
int servovlimhaut = 170;   //limite du servo vertical mouvement haut
int servovlimbas = 10;      //limite du servo vertical mouvement bas
int servonv = 30;
int posv;

//assignation des broches avec les LDR

int LDRHG = A1; // variable pour assigner LDR haut gauche au port analogue
int LDRHD = A2; // variable pour assigner LDR haut droit au port analogue
int LDRBG = A3; // variable pour assigner LDR bas gauche au port analogue
int LDRBD = A4; // variable pour assigner LDR bas droit au port analogue

//variable pour les valeurs mesuré par les LDR//

int hd;   // variable pour signal capté par LDR haut droit
int hg;   // variable pour signal capté par LDR haut gauche
int bd;   // variable pour signal capté par LDR bas droit
int bg;   // variable pour signal capté par LDR bas droit


int tolerancev = 100; //détermine la tolérance entre la différence des valeurs des LDR hd et bd
int toleranceh = 100; //détermine la tolérance entre la différence des valeurs des LDR hd et hg

void setup()   // le "setup" est exécuté une seule fois au début du programme
{
  horizontal.attach(11);      // définir le signal pour servo horizontale sur les broches 11
  horizontal.write(90);


  vertical.attach(3);         // définir le signal pour servo vertical sur les broches 3
  vertical.write(90);

}

void loop()   // la "loop" s'exécute continuellement
{

  servoh = horizontal.read(); //détermine la position du servo horizontal
  servov = vertical.read();   //détermine la position du servo vertical

  hd = analogRead(LDRHD) ; // signal capté par LDR haut gauche sur la broche analogique LDRHD
  hg = analogRead(LDRHG) ; // signal capté par LDR haut gauche sur la broche analogique LDRHG
  bd = analogRead(LDRBD) ; // signal capté par LDR haut gauche sur la broche analogique LDRBD
  bg = analogRead(LDRBG) ; // signal capté par LDR haut gauche sur la broche analogique LDRBG


  if (abs(hd - bd) > tolerancev || abs(bd - hd) > tolerancev)

    if (bd < hd)
    {
      vertical.write(servov + 1);
      if (servov > servovlimhaut)
      {
        servov = servovlimhaut;
      }
      delay(1);
    }
    else if (hd < bd)
    {
      vertical.write(servov - 1);
      if (servov < servovlimbas)
      {
        servov = servovlimbas;
      }
      delay(1);
    }
    else
    {
      vertical.write(servov);
    }

  if (abs(hd - hg) > toleranceh || abs(hg - hd) > toleranceh)

    if (hd < hg)
    {
      horizontal.write(servoh + 1);
      if (servoh > servohlimh)
      {
        servoh = servohlimh;
      }
      delay(1);
    }
    else if (hg < hd)
    {
      horizontal.write(servoh - 1);
      if (servoh < servohlimah)
      {
        servoh = servohlimah;
      }
      delay(1);
    }
    else
    {
      horizontal.write(servoh);
    }
  delay(10);

}
