//**************************************************************************************************************************
// CrcDuino Tutorial 4 - Basic power Motor_control using an encoder
// Inspired by: https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/ 
//      and     https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros/
// The code below uses the millis() native Arduino function, a command that returns the number of milliseconds since the board started running its current sketch, to measure elapsed time.
// NOTE:  To simplify code and focus on the encoder use, this version of the code does not cover the case where the Bouton_Power is still pressed when the encoder reaches its position.
//**************************************************************************************************************************

#include <CrcLib.h>

//"Encoder" library by Paul Stoffregen must first be installed via "Arduino Library Manager")
//  How-to-use https://www.pjrc.com/teensy/td_libs_Encoder.html
//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

//Renaming pins with significant names
#define Motor_control_1 CRC_PWM_1
#define Bouton_Power CRC_DIG_1

using namespace Crc;

//Motor1 encoder variable: "Encoder" library pre-requisite
Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B); //This encoder is plugged into these 2 pins on the board

//Program execution variable
bool motorRotating = LOW; // will allow Motor_1 rotation

//*****************Déclaration de la fonction de l'encodeur**********************************

void rotation_encodeur(int POSITION,int VITESSE){

   CrcLib::Update();
  Serial.println(motor1Enco.read()); //Continuously print on screen the motor position
  
  if(CrcLib::GetDigitalInput(Bouton_Power) == 0)  //Start rotation when button is pressed
  {
    motorRotating = HIGH;
  }
  
  if(motorRotating == HIGH && (motor1Enco.read() <= POSITION) ) //Encoder position not yet reached
  {
    CrcLib::SetPwmOutput(Motor_control_1,VITESSE);
  }
  else
  {
    CrcLib::SetPwmOutput(Motor_control_1,0);
  }
 }

//**************************************************************************************************************************

void setup() {
  CrcLib::Initialize();
  Serial.begin(2000000);  //High baud rate for faster printing on screen

  CrcLib::SetDigitalPinMode(Bouton_Power,INPUT);
  CrcLib::InitializePwmOutput(Motor_control_1); //Use CrcLib::InitializePwmOutput(Motor_control_1,1) to rapidly reverse this motor rotation direction all over the code
  
  //At power-up, establish a known encoder position value
  // Best practice would be to instead assign a physical limit switch indicating the zero position, and have the robot search
  // that position itself at sequence start in the void loop() part. This is called "homing".
  motor1Enco.write(0);
}

*******************************************************

void loop() {

rotation_encodeur(3500,60);


}
