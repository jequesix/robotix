#include <CrcLib.h>

using namespace Crc;

int val1Y;
int val1X;
int val2X;

bool bouton1;
bool bouton2;

void setup() {
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_3);
  CrcLib::SetPwmOutput(CRC_PWM_1, 0);
  CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  CrcLib::SetPwmOutput(CRC_PWM_3, 0);
  CrcLib::SetDigitalPinMode(CRC_DIG_1, OUTPUT);
  /*
    The rest of your setup code
    ...
  */
}
void loop() {
  CrcLib::Update();

  if(CrcLib::IsCommValid() == HIGH)
  {

  val1Y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
  val1Y = map(val1Y, -128, 127, -30, 30);

  val1X = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
  val1X = map(val1X, -128, 127, -30, 30);

  val2X = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
  val2X = map(val2X, -128, 127, -30, 30);

  CrcLib::MoveArcade(val1Y, val1X, CRC_PWM_1, CRC_PWM_2);

  CrcLib::SetPwmOutput(CRC_PWM_3, val2X);


int  valR1 = CrcLib::ReadDigitalChannel(BUTTON::R1);
int valL1 = CrcLib::ReadDigitalChannel(BUTTON::L1);

  if (valR1 == HIGH)
  {
    bouton1 = HIGH;

       if (bouton1 == HIGH) 
        {
             CrcLib::SetDigitalOutput(CRC_DIG_1, HIGH);
        }
        

          else if (bouton1 == LOW) {

          CrcLib::SetDigitalOutput(CRC_DIG_1, LOW);


          }  }


if (valL1 == HIGH){

bouton2 = HIGH;

if (bouton2 == HIGH){

CrcLib::SetDigitalOutput(CRC_DIG_1, LOW);
}
}




} 
}





  

  /*
    The rest of your looping code
    ...
  */
