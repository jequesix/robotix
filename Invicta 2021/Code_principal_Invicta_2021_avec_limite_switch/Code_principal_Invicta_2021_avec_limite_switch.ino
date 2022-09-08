#include <CrcLib.h>
#include <Encoder.h>
#include <Wire.h>
#include <SR04.h>

using namespace Crc;

//*********************Scope capteur ultrasonic****************************
unsigned char CAPTEUR;

unsigned char txbuf[10] = {0};
unsigned char rxbuf[10] = {0};


typedef enum {

  SLAVEADDR_INDEX = 0,
  PID_INDEX,
  VERSION_INDEX,

  DIST_H_INDEX,
  DIST_L_INDEX,

  TEMP_H_INDEX,
  TEMP_L_INDEX,

  CFG_INDEX,
  CMD_INDEX,
  REG_NUM

} regindexTypedef;

#define    MEASURE_MODE_PASSIVE    (0x00)
#define    MEASURE_RANG_500        (0x20)
#define    CMD_DISTANCE_MEASURE    (0x01)
unsigned char addr0 = 0x11;

//************Scope Capteur Ultrasonic bleu**********

#define TRIG_PIN CRC_DIG_6
#define ECHO_PIN CRC_PWM_6

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

//**************Scope moteur encodeur*******************

bool step0 = LOW;
bool step1 = LOW;
bool step2 = LOW;
bool step3 = LOW;
bool step4 = LOW;
bool step5 = LOW;
bool step6 = LOW;


#define Motor_control_1 CRC_PWM_1
#define Bouton_Power CRC_DIG_1
#define LimitSwitch1 CRC_DIG_2 

Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

bool motorRotating = LOW;

int GLOBALE;

//**************************Déclaration de la fonction de l'encodeur***************

void rotation_encodeur(int POSITION,char V_AVANCER){

   CrcLib::Update();

 
  if((motor1Enco.read() < POSITION) ) 
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
//****************************Déclaration des fonctions capteur ultrasonic*******************

void i2cWriteBytes(unsigned char addr_t, unsigned char Reg, unsigned char *pdata, unsigned char datalen )
{
  Wire.beginTransmission(addr_t); 
  Wire.write(Reg);             

  for (uint8_t i = 0; i < datalen; i++) {
    Wire.write(*pdata);
    pdata++;
  }

  Wire.endTransmission();   
}

void i2cReadBytes(unsigned char addr_t, unsigned char Reg, unsigned char Num )
{
  unsigned char i = 0;
  Wire.beginTransmission(addr_t); 
  Wire.write(Reg);              
  Wire.endTransmission();   
  Wire.requestFrom(addr_t, Num);
  while (Wire.available())   
  {
    rxbuf[i] = Wire.read();
    i++;
  }
}


void Capteur_sonic(){

  int16_t  dist, temp;
  txbuf[0] = CMD_DISTANCE_MEASURE;

  i2cWriteBytes(addr0, CMD_INDEX, &txbuf[0], 1 ); 
  delay(100);

  i2cReadBytes(addr0, DIST_H_INDEX, 2 );
  CAPTEUR = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  i2cReadBytes(addr0, TEMP_H_INDEX, 2 ); 
  temp = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  Serial.print("Capteur 1 : ");
  Serial.print(CAPTEUR);
  Serial.print("cm");
  Serial.print(" ------");
}

//***************Fonction Capteur Ultrasonic Bleu***************

void Capteur_sonic_bleu()
{
  Serial.print(" Capteur 2 : ");
   a=sr04.Distance();
   Serial.print(a);
   Serial.print(" cm ------- ");
   
}

void setup(){

//***************Setup Capteur Sonic***********************

  Wire.begin();
  txbuf[0] =  (MEASURE_MODE_PASSIVE | MEASURE_RANG_500);
  i2cWriteBytes(addr0, CFG_INDEX, &txbuf[0], 1 ); 
  delay(100);

//********************Setup moteur encodeur*****************************
  
 CrcLib::Initialize();
  Serial.begin(2000000);  
  CrcLib::SetDigitalPinMode(Bouton_Power,INPUT);
  CrcLib::SetDigitalPinMode(LimitSwitch1, INPUT);
  CrcLib::InitializePwmOutput(Motor_control_1);
  CrcLib::InitializePwmOutput(ECHO_PIN);
   CrcLib::SetDigitalPinMode(TRIG_PIN, INPUT);
  
  motor1Enco.write(0);
 
  step0 = HIGH; step1 = LOW; step2 = LOW; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
}


void loop() 
{

if(CrcLib::GetDigitalInput(Bouton_Power)== LOW)
{

  while(1==1)
    {
       CrcLib::Update();
       Capteur_sonic();
       Serial.print(" Valeur encodeur : ");
       Serial.print(motor1Enco.read());
       Serial.println();

  if(step0 == HIGH)
      {
        rotation_encodeur(14000,20);
        CrcLib::Update();
          if(CrcLib::GetDigitalInput(LimitSwitch1) == LOW )
          {
           motor1Enco.write(0);
           CrcLib::SetPwmOutput(Motor_control_1,0);
                         
             step0 = LOW; step1 = HIGH; step2 = LOW; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
          }
      }
if(step1 == HIGH)
      {
        rotation_encodeur(3500,20);
                  
          if(GLOBALE == 3500) 
          {
              
              step0 = LOW; step1 = LOW; step2 = HIGH; step3 = LOW; step4 = LOW; step5 = LOW; step6 = LOW;
          }
      }


      
      if(step2 == HIGH)
      {
        
        rotation_encodeur(100,20);
    
        if(GLOBALE == 100) 
        {
          
          step0 = LOW; step1 = LOW; step2 = LOW; step3 = HIGH; step4 = LOW; step5 = LOW; step6 = LOW;
        }
     
       }

     if(step3 == HIGH)
     {
 CrcLib::SetPwmOutput(Motor_control_1,60);
 
if(CAPTEUR == 10)
{
  while(1==1)
{
   CrcLib::SetPwmOutput(Motor_control_1,0);
}
}
}
}
}
}
