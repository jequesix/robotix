#include <CrcLib.h>
#include <Encoder.h>


using namespace Crc;


Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

Encoder motor2Enco(CRC_SERIAL_TXD1, CRC_SERIAL_RXD1);

#define limit CRC_DIG_1

#define limit2 CRC_DIG_2
int calibrationTerminee = 0;
int GLOBALE_ascenseur;
bool btnVirtuel;
unsigned long timeCapture = 0;
int r1;


void Position_trieur (int POSITION, char V_AVANCER, int BUFFER) {

  while (1 == 1) {

    //Serial.println(motor1Enco.read());
    if (motor2Enco.read() < POSITION - BUFFER )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, V_AVANCER);
    }
    if  (motor2Enco.read() > POSITION + BUFFER )
    {
      int V_RECULER = -1 * V_AVANCER;
      CrcLib::SetPwmOutput(CRC_PWM_9, V_RECULER);
    }
    if (motor2Enco.read() < BUFFER + POSITION && motor2Enco.read() > POSITION - BUFFER ) {
      {
        if  ((motor2Enco.read() > POSITION - BUFFER && motor2Enco.read() < POSITION) )
        {
          CrcLib::SetPwmOutput(CRC_PWM_9, 10);
        }
        if  ((motor2Enco.read() < POSITION + BUFFER && motor2Enco.read() > POSITION) )
        {
          CrcLib::SetPwmOutput(CRC_PWM_9, -10);
        }
        if ( (motor2Enco.read() < POSITION + 5 && motor2Enco.read() > POSITION - 5) )
        {
          CrcLib::SetPwmOutput(CRC_PWM_9, 0);
          break;
        }
      }
    }
  }
}



//  ****************************************************************************
void ascenseur(int POSITION, char V_AVANCER) {


  CrcLib::Update();
  if ((motor1Enco.read() < POSITION) ) //Encoder position not yet reached
  {

    CrcLib::SetPwmOutput(CRC_PWM_7, V_AVANCER);

  }
  else if  ((motor1Enco.read() > POSITION) )
  {
    int V_RECULER = -1 * V_AVANCER;
    CrcLib::SetPwmOutput(CRC_PWM_7, V_RECULER);
  }
  else if (POSITION >= POSITION - 50 && POSITION <= POSITION + 50)
  {
    CrcLib::SetPwmOutput(CRC_PWM_7, 0);

    GLOBALE_ascenseur = POSITION;


  }

}


void Calibration_ascenseur() {



  while (1 == 1) {
    if (CrcLib::GetDigitalInput(limit) == LOW)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, -50);
    }

    if (CrcLib::GetDigitalInput(limit) == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 0);
      // delay(100);
      motor1Enco.write(0);
      break;
    }

  }
}



void Calibration_trieur () {

  while (1 == 1) {


    if ( CrcLib::GetDigitalInput(limit2) == LOW )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, 20);
    }
    if ( CrcLib::GetDigitalInput(limit2) == HIGH )
    {
      CrcLib::SetPwmOutput(CRC_PWM_9, 0);

      motor2Enco.write(0);
      break;

    }
  }

  
}



void setup()
{
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_3);
  CrcLib::InitializePwmOutput(CRC_PWM_4);
  CrcLib::InitializePwmOutput(CRC_PWM_5);
  CrcLib::InitializePwmOutput(CRC_PWM_6);
  CrcLib::InitializePwmOutput(CRC_PWM_7);
  CrcLib::InitializePwmOutput(CRC_PWM_8);
  CrcLib::InitializePwmOutput(CRC_PWM_9);
  CrcLib::InitializePwmOutput(CRC_PWM_10);

  /*  pinMode(CRC_SERIAL_TX1, INPUT);
    pinMode(CRC_SERIAL_RX1, INPUT);*/

  Serial.begin(115200);

  calibrationTerminee = 0;

  //
}


void loop()
{
  if (calibrationTerminee == 0)
  {
    Calibration_trieur();
    Calibration_ascenseur();
    calibrationTerminee = 1;
  }
  CrcLib::Update();

  //Serial.println(motor2Enco.read());

  if (CrcLib::IsCommValid() == HIGH)
  {

    //////////////Roues

    int valY1 = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
    valY1 = map(valY1, -128, 127, -70, 70);

    int valX1 = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    valX1 = map(valX1, -128, 127, -55, 55);


    int valX2 = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    valX2 = map(valX2, -128, 127, -35, 35);

    CrcLib::MoveHolonomic(valY1, valX2, valX1, CRC_PWM_1, CRC_PWM_2, CRC_PWM_3, CRC_PWM_4);


    ////Boutons


    int Bouton_1 = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
    int Bouton_2 = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
    int Bouton_3 = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
    int Bouton_4 = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
    int Bouton_5 = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);
    int Bouton_6 = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);




    ////////Balai

    if (CrcLib::ReadDigitalChannel(BUTTON::HATL) == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_5, 80);
    }

    else if (CrcLib::ReadDigitalChannel(BUTTON::HATL) == LOW)
    {
      CrcLib::SetPwmOutput(CRC_PWM_5, 0);
    }

    /////////////Ascenseur

    r1 = CrcLib::ReadDigitalChannel(BUTTON::R1);
    if (motor1Enco.read() <= 10325 && r1 == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 100);
    }

    
      if (motor1Enco.read() >= 9000 && motor1Enco.read() <= 10325 && r1 == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 60);
    }

    if (motor1Enco.read() <= 10325 && r1 == LOW)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 0);
    }

    if (motor1Enco.read() >= 10325 && r1 == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 0);
    }

    if (CrcLib::ReadDigitalChannel(BUTTON::L1) == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, -100);
    }

    if(CrcLib::ReadDigitalChannel(BUTTON::L1) == HIGH && motor1Enco.read()<= 1000)
  {
    CrcLib::SetPwmOutput(CRC_PWM_7, -50);
  }

    if (CrcLib::GetDigitalInput(limit) == HIGH)
    {
      motor1Enco.write(0);
    }

    if (CrcLib::GetDigitalInput(limit) == HIGH && CrcLib::ReadDigitalChannel(BUTTON::L1) == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_7, 0);
    }

    /////////////////////////////////////Servomoteur

    if (CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP) == HIGH)
    {
      btnVirtuel = HIGH;
    }

    if (btnVirtuel == HIGH)
    {
      CrcLib::SetPwmOutput(CRC_PWM_8, 10);
    }

    else
    {
      timeCapture = millis();
      CrcLib::SetPwmOutput(CRC_PWM_8, 0);
    }

    if (millis() - timeCapture > 670)
    {
      CrcLib::SetPwmOutput(CRC_PWM_8, 0);
      btnVirtuel = LOW;
    }

    /////Positions trieur


    if (Bouton_1 == HIGH)
    {
      Position_trieur(0, 50, 100);
    }

    if (Bouton_2 == HIGH)
    {
      Position_trieur(-470, 30, 100);
    }

    if (Bouton_3 == HIGH)
    {
      Position_trieur(-1100, 50, 100);
    }

    if (Bouton_4 == HIGH)
    {
      Position_trieur(-1725, 50, 100);
    }

    if (Bouton_5 == 127)
    {
      Position_trieur(-2280, 50, 100);
    }





  }
}
