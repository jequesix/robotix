#include <CrcLib.h>
using namespace Crc;

//JOYSTICK2_X: Side
//JOYSTICK2_Y: Rotation
//JOYSTICK1_Y: Bouche
//Gachette droite: aile haut
//Gachette gauche: aile bas 

void setup() {
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::SetPwmOutput(CRC_PWM_1, 50);
}
void loop() {
  // put your main code here, to run repeatedly:
  CrcLib::Update();

}
